// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzCommonActivatableWidget.h"
#include "GameplayWidget.generated.h"

class UMatchStateGauge;
class UAbilitySystemComponent;
class UBlitzAbilitySystemComponent;
class UCommonTextBlock;
class UValueGauge;
/**
 * 必须继承自CommonActivatableWidget，这是栈管理的控件类型，必须继承自该类或者其子类 \n
 * 这样该组件就实现了接口PushWidget，RemoveWidget，ClearWidget，分别对应入栈，出栈和清空栈 \n
 * 当需要为Widget对应的ViewModel进行属性绑定时，继承I...Interface，在蓝图中实现BindAttributesToViewModels();
 */
UCLASS(meta = (DisableNativeTick))
class BLITZ_API UGameplayWidget : public UBlitzCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|ViewModel")
	void BP_BindAttributesToViewModels(UBlitzAbilitySystemComponent* OwnerASC); // 不需要在蓝图中调用，所以不需要BlueprintCallable
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|ViewModel")
	void BP_BindPlayerControllerToViewModels(ABlitzPlayerController* OwnerController); 
	
	// 父类已经有了，子类不应该再写UFUNCTION(BlueprintCallable, Category = "UI|ViewModel")
	virtual void BindAttributesToViewModels_Implementation(APlayerController* InPlayerController) override;

private:
	// 需要为BlueprintReadWrite，才能在蓝图中进行视图绑定(View Binding)，否则会提示无法进行属性存取/找不到对应Widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> PlayerNumber; // 如1号队员，PlayerNumber = 1
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> PlayerName;
	// todo: 护甲值
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UValueGauge> ArmorBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UValueGauge> HealthBar;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UMatchStateGauge> MatchStateGauge;
};
