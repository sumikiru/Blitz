// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "BlitzPlayerState.generated.h"

class ABlitzPlayerController;
class UAttributeSet;
class UBlitzAttributeSet;
class UBlitzAbilitySystemComponent;

/**
 * Base player state class used by this project.
 */
UCLASS()
class BLITZ_API ABlitzPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABlitzPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerState")
	ABlitzPlayerController* GetBlitzPlayerController() const;
	
	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerState")
	UBlitzAbilitySystemComponent* GetBlitzAbilitySystemComponent() const;
	// 拥有ASC的类，需要继承IAbilitySystemInterface并重写GetAbilitySystemComponent()方法
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerState")
	UBlitzAttributeSet* GetBlitzAttributeSet() const;

protected:
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Blitz|PlayerState")
	TObjectPtr<UBlitzAbilitySystemComponent> BlitzAbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, Category = "Blitz|PlayerState")
	TObjectPtr<UBlitzAttributeSet> BlitzAttributeSet;
};
