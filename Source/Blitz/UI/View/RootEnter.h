// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "RootEnter.generated.h"

/**
 * 刚进入游戏时的Widget，会在预构造函数中PushWidget，跳转到一个CommonActivatableWidget
 */
UCLASS(meta = (DisableNativeTick))
class BLITZ_API URootEnter : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	// virtual void NativeConstruct() override;

protected:
	// 内部需要调用PushModel方法(BP_AddWidget)，用蓝图方便些。包含Push Model + SetOwnerASC + BindAttributes
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UI|CommonActivatableWidget")
	void BP_PushTargetWidgetAndBindAttributes(UCommonActivatableWidgetStack* InWidgetStack, TSubclassOf<UCommonActivatableWidget> TargetActivatableWidgetClass);
	
	// 刚进入就PushWidget的目标
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))这样Lua访问不了，不能设置为private
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> LayerWidgetStack_Main;
};
