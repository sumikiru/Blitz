// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzGameplayAbility.h"
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
	/** Actually activate ability, do not call this directly. We'll call it from ABlitzCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetRelatedFireGunMontage();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TMap<EWeaponEquipState, TObjectPtr<UAnimMontage>> FireGunMontages;
};
