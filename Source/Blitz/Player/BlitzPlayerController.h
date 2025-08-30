// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlitzPlayerController.generated.h"

class ABlitzHUD;
class UBlitzAbilitySystemComponent;
class ABlitzPlayerState;
/**
 * 
 */
UCLASS(Config = Game, meta = (ShortTooltip = "The base player controller class used by this project."))
class BLITZ_API ABlitzPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerController")
	ABlitzPlayerState* GetBlitzPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerController")
	UBlitzAbilitySystemComponent* GetBlitzAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerController")
	ABlitzHUD* GetBlitzHUD() const;

	//~APlayerController interface
	/** 重写了PlayerController的PostProcessInput函数，所以会Tick调用ASC上的ProcessAbilityInput函数 */
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface
};
