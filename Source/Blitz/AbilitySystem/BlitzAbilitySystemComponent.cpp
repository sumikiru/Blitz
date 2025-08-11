// Copyright sumikiru. All Rights Reserved,


#include "BlitzAbilitySystemComponent.h"

#include "Blitz/BlitzLogChannels.h"

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
