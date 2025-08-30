// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "BlitzPlayerCharacter.generated.h"

struct FGameplayTag;
class UBlitzInputComponent;
class ABlitzPlayerState;
class ABlitzPlayerController;
class URootEnter;
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

	UFUNCTION(BlueprintCallable, Category = "Blitz|Character")
	ABlitzPlayerController* GetBlitzPlayerController() const;
	UFUNCTION(BlueprintCallable, Category = "Blitz|Character")
	ABlitzPlayerState* GetBlitzPlayerState() const;
	
	// 多人游戏中客户端玩家角色在此时初始化，可以绑定输入InputMappingContext（BeginPlay()中不是最佳方案）
	virtual void PawnClientRestart() override;

	// @note: 同一函数Init()在服务端/客户端行为不同，仅需要在函数内执行HasAuthority()判断。
	// UFUNCTION(Server)这种适用于客户端请求服务器（注意得Client请求Server）执行敏感操作网络开销高（需RPC传输），安全性高（Validation）
	void ServerInit();
	void ClientInit();

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
	UPROPERTY(EditDefaultsOnly, Category = "View|Aim")
	float NormalSpringArmLength = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "View|Aim")
	float AimZoomSpringArmLength = 400.f;
	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	UCameraComponent* ViewCam;
	
#pragma region Input
	UPROPERTY(VisibleDefaultsOnly, Category = "Input")
	TObjectPtr<UBlitzInputComponent> BlitzInputComponent;

	void HandleLookInput(const FInputActionValue& InputActionValue);
	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleAimInput(const FInputActionValue& InputActionValue);

	FVector GetLookRightDirection() const;
	FVector GetLookForwardDirection() const;
	FVector GetMoveForwardDirection() const;

	// 函数不能写成void Input_AbilityInputTagPressed(const FGameplayTag& InputTag) const;这和BindAction接受的参数类型有关
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
#pragma endregion

	// 类型必须为TSubclassOf<UUserWidget>而不是TSubclassOf<URootEnter>，否则CreateWidget的第二个参数类型无法转换
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> RootEnterWidgetClass;
	
	// Only create widget on the client
	void SpawnRootEnterWidget() const;

public:
	UFUNCTION(BlueprintCallable, Category = "View")
	FORCEINLINE float GetSpringArmLength() const { return CameraBoom->TargetArmLength; }
	UFUNCTION(BlueprintCallable, Category = "View")
	FORCEINLINE void SetSpringArmLength(const float InTargetArmLength) const { CameraBoom->TargetArmLength = InTargetArmLength; }
	UFUNCTION(BlueprintCallable, Category = "View")
	FORCEINLINE float GetNormalSpringArmLength() const { return NormalSpringArmLength; }
	UFUNCTION(BlueprintCallable, Category = "View")
	FORCEINLINE float GetAimZoomSpringArmLength() const { return AimZoomSpringArmLength; }
	UFUNCTION(BlueprintCallable, Category = "Input")
	FORCEINLINE UBlitzInputComponent* GetBlitzInputComponent() const { return BlitzInputComponent; }
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_HandleAimZooming();
};
