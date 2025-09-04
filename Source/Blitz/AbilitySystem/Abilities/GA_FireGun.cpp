// Copyright sumikiru. All Rights Reserved,


#include "GA_FireGun.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GA_EquipWeapon.h"
#include "Abilities//Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Blitz/BlitzLogChannels.h"
#include "Blitz/Character/BlitzCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GA_FireGun)

UGA_FireGun::UGA_FireGun()
{
	// 默认实例化策略为InstancedPerActor，适合这种多次触发且需要持久化Timer的情况
	ActivationPolicy = EBlitzAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	ShootDynamicTimerDelegate.BindDynamic(this, &ThisClass::FireGunAbilityCompleted);
}

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

		// 每次固定产生的特效/音效放在AnimMontage/Animation中伴随其同步。如果是击中到敌人身上这种动态效果，使用GameplayCue，借助GE
		// todo: 将其封装为AbilityTask，完善预测功能
		TargetAvatarCharacter->PlayWeaponAnimation(WeaponFireAnimations[TargetAvatarWeaponState]);

		// 仅仅这样写还不能播放Montage，需要让它知道应该在AnimBP的哪个位置播放
		// 详见插槽DefaultSlot/FullBodyAdditivePreAim，需要在AM和AnimBP指定插槽
		// Montage默认在Avatar上播放，武器的需要单独设置。详见前面的PlayWeaponMontage()
		UAbilityTask_PlayMontageAndWait* PlayFireGunMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("FireGun"),
			GetRelatedFireGunMontage(TargetAvatarWeaponState)
		);
		//PlayFireGunMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayFireGunMontageTask->ReadyForActivation(); // don't forget, must call it in C++

		/*UAbilityTask_WaitInputPress* WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
		WaitInputPressTask->OnPress.AddDynamic(this, &ThisClass::EndFireGunAbility);*/

		// 简易写法(Set Timer By Event)
		// 此时需要将该GA实例化策略设置为InstancedPerActor，ActivationPolicy为OnInputTriggered，且"用标签阻止能力"为空(至少不应该为自己的能力Tag)
		// Set a timer to execute delegate. Setting an existing timer will reset that timer with updated parameters.
		UKismetSystemLibrary::K2_SetTimerDelegate(
			ShootDynamicTimerDelegate,
			FireDelayTimeSecs[TargetAvatarWeaponState],
			true
		);
	}
}

// 每次播放Montage时调用，开销太高
/*EWeaponEquipState UGA_FireGun::GetCurrentWeaponEquipState()
{
	if (ABlitzCharacter* TargetAvatarCharacter = Cast<ABlitzCharacter>(GetActorInfo().AvatarActor))
	{
		if (const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarCharacter))
		{
			if (ASC->HasMatchingGameplayTag(BlitzGameplayTags::Status_EquippingWeapon_Rifle))
			{
				return EWeaponEquipState::Rifle;
			}
			if (ASC->HasMatchingGameplayTag(BlitzGameplayTags::Status_EquippingWeapon_Pistol))
			{
				return EWeaponEquipState::Pistol;
			}

			return EWeaponEquipState::Unarmed;
		}
	}

	UE_LOG(LogBlitzAbilitySystem, Error, TEXT("Cannot find related fire gun montage!"));
	return EWeaponEquipState::Unarmed;
}*/

UAnimMontage* UGA_FireGun::GetRelatedFireGunMontage(const EWeaponEquipState& CurrentWeaponEquipState)
{
	if (CurrentWeaponEquipState != EWeaponEquipState::Unarmed)
	{
		return FireGunMontages[CurrentWeaponEquipState];
	}

	return nullptr;
}

void UGA_FireGun::EndFireGunAbility(float TimeWaited)
{
	K2_EndAbility();

	UE_LOG(LogTemp, Display, TEXT("FireGun Ended"));
}

void UGA_FireGun::FireGunAbilityCompleted()
{
	K2_EndAbility();

	BulletCounts++;

	UE_LOG(LogTemp, Display, TEXT("FireGun Ended: %d"), BulletCounts);
}
