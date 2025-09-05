// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MatchStateGauge.generated.h"

class UCommonTextBlock;
/**
 * 屏幕中心正上方，显示比赛得分(如50:49)以及倒计时
 */
UCLASS(meta = (DisableNativeTick))
class BLITZ_API UMatchStateGauge : public UCommonUserWidget
{
	GENERATED_BODY()

private:
	// 需要为BlueprintReadWrite，才能在蓝图中进行视图绑定(View Binding)
	// 比赛倒计时
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> MatchCountdownText;
};
