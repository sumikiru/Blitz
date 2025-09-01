// Copyright sumikiru. All Rights Reserved,


#include "GA_EquipWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities//Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Blitz/BlitzGameplayTags.h"
#include "Blitz/BlitzLogChannels.h"
#include "GameplayTagContainer.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/Character/BlitzCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_EquipWeapon)

UGA_EquipWeapon::UGA_EquipWeapon()
{
	ActivationPolicy = EBlitzAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

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

		UpdateStatusTag(TargetAvatarCharacter);

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

/**
 * 也可以借助AN_SendGameplayEvent更改
 */
void UGA_EquipWeapon::UpdateStatusTag(ABlitzCharacter* TargetAvatarCharacter)
{
	// 遍历并移除所有以"Status_EquippingWeapon"为前缀的Tag
	FGameplayTagContainer AllEquippingTags;
	AllEquippingTags.AddTag(BlitzGameplayTags::Status_EquippingWeapon_Pistol);
	AllEquippingTags.AddTag(BlitzGameplayTags::Status_EquippingWeapon_Rifle);

	// 获取角色当前拥有的标签
	FGameplayTagContainer OwnedTags;
	if (const UBlitzAbilitySystemComponent* TargetASC = TargetAvatarCharacter->GetBlitzAbilitySystemComponent())
	{
		TargetASC->GetOwnedGameplayTags(OwnedTags);
	}

	// 找出交集（即实际存在且需要移除的标签）
	const FGameplayTagContainer& TagsToRemove = OwnedTags.Filter(AllEquippingTags);

	// 移除筛选出的Tag（如果标签不存在则不做处理）
	if (!TagsToRemove.IsEmpty())
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(TargetAvatarCharacter, TagsToRemove, true);
	}

	const FGameplayTagContainer PistolTagContainer(BlitzGameplayTags::Status_EquippingWeapon_Pistol);
	const FGameplayTagContainer RifleTagContainer(BlitzGameplayTags::Status_EquippingWeapon_Rifle);

	// 添加新的Status_标签
	switch (WeaponEquipState)
	{
	case EWeaponEquipState::Pistol:
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(TargetAvatarCharacter, PistolTagContainer, true);
		break;
	case EWeaponEquipState::Rifle:
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(TargetAvatarCharacter, RifleTagContainer, true);
		break;
	case EWeaponEquipState::Unarmed:
		break;
	}

	TargetAvatarCharacter->OnEquipWeaponStateChangedDelegate.Broadcast(WeaponEquipState);
}
