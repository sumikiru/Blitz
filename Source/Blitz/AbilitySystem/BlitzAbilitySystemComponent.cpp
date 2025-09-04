// Copyright sumikiru. All Rights Reserved,


#include "BlitzAbilitySystemComponent.h"

#include "Abilities/BlitzGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "Blitz/BlitzGameplayTags.h"
#include "Blitz/BlitzLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzAbilitySystemComponent)

void UBlitzAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		ABILITYLIST_SCOPE_LOCK();
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
	else
	{
		UE_LOG(LogBlitzAbilitySystem, Warning, TEXT("Invalid Gameplay Tag [%s]"), *InputTag.ToString())
	}
}

void UBlitzAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		ABILITYLIST_SCOPE_LOCK();
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
	else
	{
		UE_LOG(LogBlitzAbilitySystem, Warning, TEXT("Invalid Gameplay Tag [%s]"), *InputTag.ToString())
	}
}

void UBlitzAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(BlitzGameplayTags::Status_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	// 注意static
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset(); // 注意！

	// Process all abilities that activate when the input is held
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UBlitzGameplayAbility* BlitzAbilityCDO = CastChecked<UBlitzGameplayAbility>(AbilitySpec->Ability);

				if (BlitzAbilityCDO->GetActivationPolicy() == EBlitzAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	// Process all abilities that had their input pressed this frame
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UBlitzGameplayAbility* BlitzAbilityCDO = CastChecked<UBlitzGameplayAbility>(AbilitySpec->Ability);

					if (BlitzAbilityCDO->GetActivationPolicy() == EBlitzAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	/**
	 * Try to activate all the abilities that are from presses and holds.
	 * We do it all at once so that held inputs don't activate the ability
	 * and then also send a input event to the ability because of the press.
	 */
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	// Process all abilities that had their input released this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// Clear the cached ability handles.
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UBlitzAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UBlitzAbilitySystemComponent::ApplyGameplayEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : Effects)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffectClass, 1.f, MakeEffectContext());
		if (EffectSpecHandle.IsValid() && EffectSpecHandle.Data.IsValid())
		{
			ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
		else
		{
			UE_LOG(LogBlitz, Error, TEXT("Failed to apply gameplay effect, please check if the effect is set in the pawn data"));
		}
	}
}

void UBlitzAbilitySystemComponent::ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(),
		GameplayCueTag,
		EGameplayCueEvent::Executed,
		GameplayCueParameters
	);
}

void UBlitzAbilitySystemComponent::AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(),
		GameplayCueTag,
		EGameplayCueEvent::OnActive,
		GameplayCueParameters
	);
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(),
		GameplayCueTag,
		EGameplayCueEvent::WhileActive,
		GameplayCueParameters
	);
}

void UBlitzAbilitySystemComponent::RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(),
		GameplayCueTag,
		EGameplayCueEvent::Removed,
		GameplayCueParameters
	);
}

void UBlitzAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
		// Invoke the InputPressed event. This is not replicated here.
		// If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UBlitzAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}
