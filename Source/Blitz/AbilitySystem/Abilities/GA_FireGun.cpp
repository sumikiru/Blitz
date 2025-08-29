// Copyright sumikiru. All Rights Reserved,


#include "GA_FireGun.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GA_EquipWeapon.h"
#include "Abilities//Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Blitz/BlitzGameplayTags.h"
#include "Blitz/BlitzLogChannels.h"
#include "Blitz/Character/BlitzCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_FireGun)

void UGA_FireGun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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

		// 仅仅这样写还不能播放Montage，需要让它知道应该在AnimBP的哪个位置播放
		// 详见插槽DefaultSlot/FullBodyAdditivePreAim，需要在AM和AnimBP指定插槽
		UAbilityTask_PlayMontageAndWait* PlayFireGunMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("FireGun"),
			GetRelatedFireGunMontage()
		);
		PlayFireGunMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->ReadyForActivation(); // don't forget, must call it in C++
		
		/*UAbilityTask_PlayAnimAndWait* PlayGunFireAnimSequenceTask = UAbilityTask_PlayAnimAndWait::CreatePlayAnimAndWaitProxy(
			this,
			FName("FireGun"),
			GunFireAnimSequence,
			FName("LyraMovement.FullBodyAdditivePreAim")	// 注意！与AnimBP以及Montage的插槽名对应，包括其组名LyraMovement
		);
		PlayGunFireAnimSequenceTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGunFireAnimSequenceTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGunFireAnimSequenceTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGunFireAnimSequenceTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayGunFireAnimSequenceTask->ReadyForActivation();*/
	}
}

UAnimMontage* UGA_FireGun::GetRelatedFireGunMontage()
{
	if (ABlitzCharacter* TargetAvatarCharacter = Cast<ABlitzCharacter>(GetActorInfo().AvatarActor))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarCharacter))
		{
			if (ASC->HasMatchingGameplayTag(BlitzGameplayTags::Status_EquippingWeapon_Rifle))
			{
				return FireGunMontages[EWeaponEquipState::Rifle];
			}
			if (ASC->HasMatchingGameplayTag(BlitzGameplayTags::Status_EquippingWeapon_Pistol))
			{
				return FireGunMontages[EWeaponEquipState::Pistol];
			}
		}
	}

	UE_LOG(LogBlitzAbilitySystem, Error, TEXT("Cannot find related fire gun montage!"));
	return nullptr;
}
