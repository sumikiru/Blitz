// Copyright sumikiru. All Rights Reserved,


#include "GA_WeaponReload.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_WeaponReload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 重点：仅在权威端或本地预测
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		// note
		if (!K2_CommitAbility())
		{
			K2_EndAbility();
			return;
		}

		UAbilityTask_PlayMontageAndWait* PlayWeaponReloadMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("WeaponReload"),
			WeaponReloadMontage
		);
		PlayWeaponReloadMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->ReadyForActivation(); // don't forget, must call it in C++
	}
}
