// Copyright sumikiru. All Rights Reserved,


#include "GA_FireGun.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_FireGun)

void UGA_FireGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// note
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("FireGun Ability Activated"));
}
