// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlitzCharacter.generated.h"

class UInputComponent;

UCLASS()
class BLITZ_API ABlitzCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlitzCharacter();

protected:
	virtual void BeginPlay() override;

};
