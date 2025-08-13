// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayWidget.generated.h"

class UCommonTextBlock;
class UValueGauge;
/**
 * 必须继承自CommonActivatableWidget，这是栈管理的控件类型，必须继承自该类或者其子类
 * 这样该组件就实现了接口PushWidget，RemoveWidget，ClearWidget，分别对应入栈，出栈和清空栈
 */
UCLASS(meta = (DisableNativeTick))
class BLITZ_API UGameplayWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> PlayerName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UValueGauge> HealthBar;
	// todo: 耐力条
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UValueGauge> StaminaBar;
};
