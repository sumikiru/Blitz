// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzGameplayAbility.h"
#include "GA_EquipWeapon.h"
#include "GA_FireGun.generated.h"

enum class EWeaponEquipState : uint8;
/**
 * 
 */
UCLASS()
class BLITZ_API UGA_FireGun : public UBlitzGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_FireGun();
	
	/** Actually activate ability, do not call this directly. We'll call it from ABlitzCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void UpdateCurrentWeaponEquipState(EWeaponEquipState NewWeaponEquipState);
	
protected:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetRelatedFireGunMontage();
	
private:
	// WaitInputPressTask->OnPress绑定的函数需要一个参数，同时还需要UFUNCTION()宏标记
	UFUNCTION()
	void EndFireGunAbility(float TimeWaited);
	UFUNCTION()
	void FireGunAbilityCompleted();
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TMap<EWeaponEquipState, TObjectPtr<UAnimMontage>> FireGunMontages;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TMap<EWeaponEquipState, float> FireDelayTimeSecs;	// 射击间隔
	// 缓存当前Weapon，如果每次ActivateAbility时播放Montage都去获取ASC然后判断Tag，会造成大量开销。借助Delegate实现
	UPROPERTY(VisibleAnywhere, Category = "Animation")
	EWeaponEquipState CurrentWeaponEquipState = EWeaponEquipState::Pistol;

	FTimerDynamicDelegate ShootDynamicTimerDelegate;

	int32 BulletCounts = 0;
};
