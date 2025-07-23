// Copyright sumikiru. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

BLITZ_API DECLARE_LOG_CATEGORY_EXTERN(LogBlitz, Log, All);
BLITZ_API DECLARE_LOG_CATEGORY_EXTERN(LogBlitzAbilitySystem, Log, All);

BLITZ_API FString GetClientServerContextString(UObject* ContextObject = nullptr);