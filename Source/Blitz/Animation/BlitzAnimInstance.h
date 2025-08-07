// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlitzAnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * The base game animation instance class used by this project.
 * https://zhuanlan.zhihu.com/p/517368184
 */
UCLASS(Config = Game)
class BLITZ_API UBlitzAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	/** 获取并更新相关Data */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	/** 为保证性能，使用多线程动画来计算动画值，而不是使用事件图表(AnimGraph)，在这里调用函数进行逻辑处理 */
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
	// 注意函数需要是线程安全的，会在ThreadSafeUpdateAnimation中调用
	/*UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateVelocityData();*/
};
