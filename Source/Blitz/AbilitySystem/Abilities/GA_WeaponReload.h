// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzGameplayAbility.h"
#include "GA_WeaponReload.generated.h"

/**
 * 
 */
UCLASS()
class BLITZ_API UGA_WeaponReload : public UBlitzGameplayAbility
{
	GENERATED_BODY()

public:
	/** Actually activate ability, do not call this directly. We'll call it from ABlitzCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> WeaponReloadMontage;
};
