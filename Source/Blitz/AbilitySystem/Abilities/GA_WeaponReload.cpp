// Copyright sumikiru. All Rights Reserved,


#include "GA_WeaponReload.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GA_EquipWeapon.h"
#include "Blitz/BlitzLogChannels.h"
#include "Blitz/Character/BlitzCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_WeaponReload)

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

		// 不能在构造函数中调用，无法获取GetActorInfo()
		ABlitzCharacter* TargetAvatarCharacter = Cast<ABlitzCharacter>(GetActorInfo().AvatarActor);
		if (!TargetAvatarCharacter)
		{
			UE_LOG(LogBlitzAbilitySystem, Error, TEXT("Invalid Target Avatar Character!"));
			K2_EndAbility();
			return;	// 别忘了return;
		}
		
		const EWeaponEquipState& TargetAvatarWeaponState = TargetAvatarCharacter->CurrentEquipWeaponState;
		if (TargetAvatarWeaponState == EWeaponEquipState::Unarmed)
		{
			UE_LOG(LogBlitzAbilitySystem, Error, TEXT("Cannot find related weapon reload montage!"));
			K2_EndAbility();
			return;
		}

		TargetAvatarCharacter->PlayWeaponAnimation(WeaponAnimations[TargetAvatarWeaponState]);

		UAbilityTask_PlayMontageAndWait* PlayWeaponReloadMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("WeaponReload"),
			GetRelatedWeaponReloadMontage(TargetAvatarWeaponState)
		);
		PlayWeaponReloadMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayWeaponReloadMontageTask->ReadyForActivation(); // don't forget, must call it in C++
	}
}

UAnimMontage* UGA_WeaponReload::GetRelatedWeaponReloadMontage(const EWeaponEquipState& CurrentWeaponEquipState)
{
	/*if (ABlitzCharacter* TargetAvatarCharacter = Cast<ABlitzCharacter>(GetActorInfo().AvatarActor))
	{
		if (const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarCharacter))
		{
			if (ASC->HasMatchingGameplayTag(BlitzGameplayTags::Status_EquippingWeapon_Rifle))
			{
				return CharacterReloadMontages[EWeaponEquipState::Rifle];
			}
			if (ASC->HasMatchingGameplayTag(BlitzGameplayTags::Status_EquippingWeapon_Pistol))
			{
				return CharacterReloadMontages[EWeaponEquipState::Pistol];
			}
		}
	}*/

	if (CurrentWeaponEquipState != EWeaponEquipState::Unarmed)
	{
		return CharacterReloadMontages[CurrentWeaponEquipState];
	}
	
	UE_LOG(LogBlitzAbilitySystem, Error, TEXT("Cannot find related weapon reload montage!"));
	return nullptr;
}
