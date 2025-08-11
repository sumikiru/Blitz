// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "BlitzHealthComponent.generated.h"

struct FGameplayEffectSpec;
/**
 * An actor component used to handle anything related to health.
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class BLITZ_API UBlitzHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UBlitzHealthComponent(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Blitz|Health")
	static UBlitzHealthComponent* FindHealthComponent(const AActor* Actor);

protected:
	/*virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
	                                 float OldValue, float NewValue);
	virtual void HandleMaxHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
	                                    float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude,
	                               float OldValue, float NewValue);*/
};
