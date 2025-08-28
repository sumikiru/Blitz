// Copyright sumikiru. All Rights Reserved,


#include "GA_EquipWeapon.h"
#include "Abilities//Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities//Tasks/AbilityTask_PlayAnimAndWait.h"
#include "Blitz/BlitzLogChannels.h"
#include "Blitz/Character/BlitzCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_EquipWeapon)

void UGA_EquipWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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

		EquipWeapon();

		UAbilityTask_PlayMontageAndWait* PlayFireGunMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("EquipWeapon"),
			EquipWeaponMontage
		);
		PlayFireGunMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->ReadyForActivation(); // don't forget, must call it in C++
	}
}

void UGA_EquipWeapon::EquipWeapon()
{
	if (ABlitzCharacter* TargetAvatarCharacter = Cast<ABlitzCharacter>(GetActorInfo().AvatarActor))
	{
		// 直接调用这个函数无法同步
		// TargetAvatarCharacter->GetMesh()->LinkAnimClassLayers(WeaponAnimLayer);
		// 可以同步，Server调用NetMulticast版本
		// TargetAvatarCharacter->LinkNewAnimClassLayers(WeaponAnimLayer);

		FName PistolAttachSocket;
		FName RifleAttachSocket;

		switch (WeaponEquipState)
		{
		case EWeaponEquipState::Pistol:
			TargetAvatarCharacter->LinkNewAnimClassLayers(TargetAvatarCharacter->PistolLayer);
			PistolAttachSocket = FName("WeaponEquipped");
			RifleAttachSocket = FName("RifleUnequipped");
			break;
		case EWeaponEquipState::Rifle:
			//TargetAvatarCharacter->LinkNewAnimClassLayers(TargetAvatarCharacter->RifleLayer);
			TargetAvatarCharacter->LinkNewAnimClassLayers(TargetAvatarCharacter->RifleLayer);
			PistolAttachSocket = FName("PistolUnequipped");
			RifleAttachSocket = FName("WeaponEquipped");
			break;
		case EWeaponEquipState::Unarmed:
			TargetAvatarCharacter->LinkNewAnimClassLayers(TargetAvatarCharacter->UnarmedLayer);
			PistolAttachSocket = FName("PistolUnequipped");
			RifleAttachSocket = FName("RifleUnequipped");
		}

		if (TargetAvatarCharacter->GetMesh()->DoesSocketExist(PistolAttachSocket))
		{
			TargetAvatarCharacter->GetPistolMeshComponent()->AttachToComponent(
				TargetAvatarCharacter->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				PistolAttachSocket
			);
		}
		else
		{
			UE_LOG(LogBlitzAbilitySystem, Warning, TEXT("Pistol Mesh Component attach to character skeletal mesh failed.please check socket name [%s]."),
			       *PistolAttachSocket.ToString());
		}

		if (TargetAvatarCharacter->GetMesh()->DoesSocketExist(RifleAttachSocket))
		{
			TargetAvatarCharacter->GetRifleMeshComponent()->AttachToComponent(
				TargetAvatarCharacter->GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				RifleAttachSocket
			);
		}
		else
		{
			UE_LOG(LogBlitzAbilitySystem, Warning, TEXT("Rifle Mesh Component attach to character skeletal mesh failed.please check socket name [%s]."),
			       *RifleAttachSocket.ToString());
		}
	}
}
