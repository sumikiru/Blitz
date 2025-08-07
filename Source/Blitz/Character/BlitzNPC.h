// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzCharacter.h"
#include "BlitzNPC.generated.h"

/**
 * 
 */
UCLASS()
class BLITZ_API ABlitzNPC : public ABlitzCharacter
{
	GENERATED_BODY()

public:
	ABlitzNPC();
	// 对于AI控制的角色，ASC存在于Pawn中，通常在Pawn的BeginPlay()方法中完成ASC在服务器端和客户端的初始化
	virtual void BeginPlay() override;

protected:
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
};
