// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ViewModelBindingInterface.generated.h"

class ABlitzPlayerController;
class UBlitzAbilitySystemComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UViewModelBindingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLITZ_API IViewModelBindingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// virtual void BindAttributes(UBlitzAbilitySystemComponent* InASC) = 0;
	UFUNCTION(BlueprintNativeEvent)
	void BindAttributes(UBlitzAbilitySystemComponent* InASC);
	UFUNCTION(BlueprintNativeEvent)
	void BindPlayerController(ABlitzPlayerController* InPlayerController);
};
