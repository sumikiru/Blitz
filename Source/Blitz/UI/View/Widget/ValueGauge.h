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

	// 仅作了解，无法在View Binding中使用，应该使用VM_PercentBarBase中的GetValuePercent()和GetValueText();
	void SetValue(float NewValue, float NewMaxValue) const;

private:
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor BarColor;
	// 需要为BlueprintReadWrite，才能在蓝图中进行视图绑定(View Binding)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UProgressBar> ProgressBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> ValueText;
};
