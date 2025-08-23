// Copyright sumikiru. All Rights Reserved,


#include "BlitzInputConfig.h"

#include "Blitz/BlitzLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzInputConfig)

UBlitzInputConfig::UBlitzInputConfig()
{
}

const UInputAction* UBlitzInputConfig::FindNativeInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FBlitzInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBlitz, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

const UInputAction* UBlitzInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FBlitzInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBlitz, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
