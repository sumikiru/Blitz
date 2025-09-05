// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

class ABlitzPlayerController;
class ABlitzCharacter;

/**
 * 记录BoxComponent的位置和朝向
 */
USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector BoxExtent;
};

/**
 * 存储特定帧(时间戳)的HitBox信息
 */
USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float Time;

	// BoneName对应BoxComponent
	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;
};

/**
 * 延迟补偿组件，参考https://zhuanlan.zhihu.com/p/1929071131677143054
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLITZ_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULagCompensationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ShowDebugFramePackage(const FFramePackage& Package, const FColor& Color = FColor::Red) const;

protected:
	virtual void BeginPlay() override;
	// Only call this on the server
	void SaveFramePackage(FFramePackage& Package) const;

private:
	// 在ABlitzCharacter::PostInitializeComponents()中初始化
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABlitzCharacter> OwnerCharacter;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABlitzPlayerController> OwnerPlayerController;

public:
	FORCEINLINE void SetOwner(ABlitzCharacter* InCharacter) { OwnerCharacter = InCharacter; }
	FORCEINLINE void SetOwnerController(ABlitzPlayerController* InPlayerController) { OwnerPlayerController = InPlayerController; }
};
