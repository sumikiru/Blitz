// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BlitzGameplayAbility.generated.h"

/**
 * The base gameplay ability class used by this project.
 */
UCLASS(Abstract, meta = (ShortToolTip = "The base gameplay ability class used by this project."))
class BLITZ_API UBlitzGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
};
