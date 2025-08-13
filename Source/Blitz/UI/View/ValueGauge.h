// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonTextBlock.h"
#include "ValueGauge.generated.h"

class UProgressBar;
/**
 * 启用CommonUI详见https://zhuanlan.zhihu.com/p/698935180
 * 注意DisableNativeTick
 */
UCLASS(meta = (DisableNativeTick))
class BLITZ_API UValueGauge : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	// 在预构造函数中初始化组件
	virtual void NativePreConstruct() override;
	
	void SetValue(float NewValue, float NewMaxValue);

private:
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor BarColor;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> ValueText;
};
