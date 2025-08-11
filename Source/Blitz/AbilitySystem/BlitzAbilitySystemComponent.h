// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BlitzAbilitySystemComponent.generated.h"

/**
 * Base ability system component class used by this project.
 */
UCLASS()
class BLITZ_API UBlitzAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Only called on the server
	void ApplyGameplayEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects);
};
