// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzGameplayAbility.h"
#include "GA_EquipWeapon.generated.h"

class ABlitzCharacter;

UENUM(BlueprintType)
enum class EWeaponEquipState : uint8
{
	Unarmed,
	Pistol,
	Rifle
};

/**
 * 进入对局时自动激活，以及切换武器时
 */
UCLASS()
class BLITZ_API UGA_EquipWeapon : public UBlitzGameplayAbility
{
	GENERATED_BODY()

public:
	/** Actually activate ability, do not call this directly. We'll call it from ABlitzCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void EquipWeapon();
	
	/**
	 * 错误代码：Gameplay能力不会复制到模拟代理，因此NetMulticast函数Multicast_AttachWeapon毫无意义
	 * UFUNCTION(NetMulticast, Reliable)
	 * void Multicast_AttachWeapon();
	*/
	
private:
	/** 更新当前玩家状态标签，为Reload实现条件做准备 */
	UFUNCTION(BlueprintCallable)
	void UpdateStatusTag(ABlitzCharacter* TargetAvatarCharacter);
	
	// 注意，需要设置其插槽为"FName("LyraMovement.FullBodyAdditivePreAim")"，和AnimBP声明的插槽对应，才能正常播放
	// 如果是AnimSequence，则需要在CreatePlayMontageAndWaitProxy中指定（前提是该AnimSequence中有该插槽），建议使用Montage不易出错
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> EquipWeaponMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	EWeaponEquipState WeaponEquipState = EWeaponEquipState::Pistol;
};
