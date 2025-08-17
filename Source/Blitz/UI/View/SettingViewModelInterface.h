// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SettingViewModelInterface.generated.h"

class UAbilitySystemComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USettingViewModelInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 用于将Widget绑定的ViewModel与Attribute绑定
 */
class BLITZ_API ISettingViewModelInterface
{
	GENERATED_BODY()

public:
	// virtual void BindAttributesToViewModels() = 0;
	UFUNCTION(BlueprintNativeEvent)
	void BindAttributesToViewModels(UAbilitySystemComponent* InASC);
};
