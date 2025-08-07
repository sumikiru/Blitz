// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzCharacter.h"
#include "BlitzPlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class BLITZ_API ABlitzPlayerCharacter : public ABlitzCharacter
{
	GENERATED_BODY()

public:
	ABlitzPlayerCharacter();
	// 多人游戏中客户端玩家角色在此时初始化，可以绑定输入InputMappingContext（BeginPlay()中不是最佳方案）
	virtual void PawnClientRestart() override;

protected:
	// 服务端初始化ASC
	virtual void PossessedBy(AController* NewController) override;
	// 客户端初始化ASC
	virtual void OnRep_PlayerState() override;

	// 初始化Player的时候，从PlayerState中获取ASC和AS，用以初始化Player的ASC和AS
	virtual void InitBlitzAbilityActorInfo();
	// 在这里绑定InputAction对应的UFUNCTION（IMC中是绑定对应的按键）
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	UCameraComponent* ViewCam;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* GameplayInputMappingContext;

	void HandleLookInput(const FInputActionValue& InputActionValue);
	void HandleMoveInput(const FInputActionValue& InputActionValue);

	FVector GetLookRightDirection() const;
	FVector GetLookForwardDirection() const;
	FVector GetMoveForwardDirection() const;
};
