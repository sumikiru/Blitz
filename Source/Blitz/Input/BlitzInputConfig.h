// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BlitzInputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * Struct used to map an input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FBlitzInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * Struct used to map input actions to keyboard.
 */
USTRUCT(BlueprintType)
struct FBlitzInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 Priority = 0;
};

/**
 * Non-mutable data asset that contains input configuration properties.
 */
UCLASS(BlueprintType, Const)
class BLITZ_API UBlitzInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindNativeInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FBlitzInputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FBlitzInputAction> AbilityInputActions;

	// List of input mapping contexts used by the owner.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBlitzInputMappingContextAndPriority> InputMappingContextAndPriorities;
};
