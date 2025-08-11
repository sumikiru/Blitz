// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BlitzPawnData.generated.h"

class UGameplayEffect;
class ABlitzCharacter;
class UBlitzInputConfig;
/**
 * Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, meta = (DisplayName = "Blitz Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class BLITZ_API UBlitzPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UBlitzPawnData(const FObjectInitializer& ObjectInitializer);

	// Class to instantiate for this pawn (should usually derive from ABlitzCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blitz|Character")
	TSubclassOf<ABlitzCharacter> CharacterClass;

	/*// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blitz|Abilities")
	TArray<TObjectPtr<UBlitzAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blitz|Abilities")
	TObjectPtr<UBlitzAbilityTagRelationshipMapping> TagRelationshipMapping;*/

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blitz|Input")
	TObjectPtr<UBlitzInputConfig> InputConfig;
	
	// Effects to apply to this pawn on spawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blitz|Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;
};
