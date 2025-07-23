// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "BlitzHeroComponent.generated.h"

class UBlitzInputConfig;
/**
 * Component that sets up input and camera handling for player controlled pawns (or bots that simulate players).
 * This depends on a PawnExtensionComponent to coordinate initialization.
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class BLITZ_API UBlitzHeroComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UBlitzHeroComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Blitz|Hero")
	static UBlitzHeroComponent* FindHeroComponent(const AActor* Actor);

	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const UBlitzInputConfig* InputConfig);
	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const UBlitzInputConfig* InputConfig);
	
protected:
	/**
	 * Input Configs that should be added to this player when initializing the input. These configs
	 * will NOT be registered with the settings because they are added at runtime. If you want the config
	 * pair to be in the settings, then add it via the GameFeatureAction_AddInputConfig
	 * 
	 * NOTE: You should only add to this if you do not have a game feature plugin accessible to you.
	 * If you do, then use the GameFeatureAction_AddInputConfig instead. 
	 */
	//UPROPERTY(EditAnywhere)
	//TArray<FInputMappingContextAndPriority> DefaultInputMappings;
	
	/** True when player input bindings have been applied, will never be true for non-players */
	bool bReadyToBindInputs;
};
