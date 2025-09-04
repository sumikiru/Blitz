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
	
protected:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetRelatedFireGunMontage(const EWeaponEquipState& CurrentWeaponEquipState);
	
private:
	// WaitInputPressTask->OnPress绑定的函数需要一个参数，同时还需要UFUNCTION()宏标记
	UFUNCTION()
	void EndFireGunAbility(float TimeWaited);
	UFUNCTION()
	void FireGunAbilityCompleted();
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TMap<EWeaponEquipState, TObjectPtr<UAnimMontage>> FireGunMontages;	// Character
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TMap<EWeaponEquipState, TObjectPtr<UAnimationAsset>> WeaponFireAnimations;	// Weapons
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TMap<EWeaponEquipState, float> FireDelayTimeSecs;	// 射击间隔

	FTimerDynamicDelegate ShootDynamicTimerDelegate;

	int32 BulletCounts = 0;
};
