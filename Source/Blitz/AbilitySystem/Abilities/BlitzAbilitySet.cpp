// Copyright sumikiru. All Rights Reserved,


#include "BlitzAbilitySet.h"

#include "ActiveGameplayEffectHandle.h"
#include "BlitzGameplayAbility.h"
#include "GameplayAbilitySpecHandle.h"
#include "Blitz/BlitzLogChannels.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/AbilitySystem/Attributes/BlitzAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzAbilitySet)

void FBlitzAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FBlitzAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FBlitzAbilitySet_GrantedHandles::AddAttributeSet(UBlitzAttributeSet* BlitzAS)
{
	GrantedAttributeSets.Add(BlitzAS);
}

void FBlitzAbilitySet_GrantedHandles::RemoveFromAbilitySystem(UBlitzAbilitySystemComponent* BlitzASC)
{
	checkf(BlitzASC, TEXT("BlitzASC is not valid in FBlitzAbilitySet_GrantedHandles::TakeFromAbilitySystem"));

	// @note: Must be authoritative to give or take ability sets.
	if (!BlitzASC->IsOwnerActorAuthoritative())
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("Attempting to remove ability set from non-authoritative actor [%s]"),
		       *BlitzASC->GetOwner()->GetName());
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			BlitzASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			BlitzASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UBlitzAttributeSet* AS : GrantedAttributeSets)
	{
		// Remove an existing attribute set
		BlitzASC->RemoveSpawnedAttribute(AS);
	}

	// Reset: Same as empty, but doesn't change memory allocations
	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

void UBlitzAbilitySet::GiveToAbilitySystemComponent(UBlitzAbilitySystemComponent* BlitzASC, FBlitzAbilitySet_GrantedHandles* OutGrantedHandles,
                                                    UObject* SourceObject) const
{
	checkf(BlitzASC, TEXT("BlitzASC is not valid in [%s]::GiveToAbilitySystemComponent()"), *GetNameSafe(this));

	// @note: Must be authoritative to give or take ability sets.
	if (!BlitzASC->IsOwnerActorAuthoritative())
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("Attempting to remove ability set from non-authoritative actor [%s]"),
		       *BlitzASC->GetOwner()->GetName());
		return;
	}

	/*********************************** Grant the gameplay abilities *********************************************/
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); AbilityIndex++)
	{
		const FBlitzAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		
		/**
		 * @note: 如果遇到“类型UGameplayAbility不完整”这种不允许指针指向不完整的类类型的提示，说明缺少头文件，导入"BlitzGameplayAbility.h"即可
		 */
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogBlitzAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UBlitzGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UBlitzGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = BlitzASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	/************************************ Grant the gameplay effects **********************************************/
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); EffectIndex++)
	{
		const FBlitzAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogBlitzAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid."), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();

		const FActiveGameplayEffectHandle GameplayEffectHandle = BlitzASC->ApplyGameplayEffectToSelf(
			GameplayEffect,
			EffectToGrant.EffectLevel,
			BlitzASC->MakeEffectContext()
		);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	/************************************* Grant the attribute sets ***********************************************/
	for (int32 AttributeSetIndex = 0; AttributeSetIndex < GrantedAttributes.Num(); AttributeSetIndex++)
	{
		const FBlitzAbilitySet_AttributeSet& AttributeSetToGrant = GrantedAttributes[AttributeSetIndex];

		if (!IsValid(AttributeSetToGrant.AttributeSet))
		{
			UE_LOG(LogBlitzAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid."), AttributeSetIndex, *GetNameSafe(this));
			continue;
		}

		// 和前面GetDefaultObject<T>的区别在于，NewObject创建了一个新的实例，而不是获取默认对象。避免了多个Character共用同一个AS，进行同样的属性变化
		// GA和GE不一样，不会互相干涉
		UBlitzAttributeSet* NewAttributeSet = NewObject<UBlitzAttributeSet>(BlitzASC->GetOwner(), AttributeSetToGrant.AttributeSet);
		
		BlitzASC->AddAttributeSetSubobject(NewAttributeSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewAttributeSet);
		}
	}
}
