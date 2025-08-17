// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blitz/UI/View/SettingViewModelInterface.h"
#include "BlitzCommonActivatableWidget.generated.h"

class UBlitzAbilitySystemComponent;
/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class BLITZ_API UBlitzCommonActivatableWidget : public UCommonActivatableWidget, public ISettingViewModelInterface
{
	GENERATED_BODY()

protected:
	/**
	 * @note: ViewModel的实例化发生在预构造和构造函数之间，应该在NativeConstruct()中执行该函数\n
	 * 由于虚函数不能使用BlueprintImplementableEvent，所以只能借助BP_BindAttributesToViewModels()\n
	 * 之所以使用BP_BindAttributesToViewModels()，是因为要对其View Binding对应的ViewModel进行BindAttributes();
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|ViewModel")
	virtual void BindAttributesToViewModels_Implementation(UAbilitySystemComponent* InASC) override;
	
	/**
	 * 参考自https://stackoverflow.com/questions/66605075/ue4-userwidget-button-bind-with-spawning-actor-in-playercontroller \n
	 * 不能通过GetOwningPlayer来获取ASC，因为前提是要SetOwningPlayer，这一步需要手动实现！ \n
	 * @note 应根据WBP_Root中Push Widget得到的UCommonActivatableWidget，之后才SetOwnerAbilitySystemComponent+BindAttributesToViewModels，
	 * 不能在NativeConstruct()/NativeInitialized()等等中实现，此时会提示无效的InASC，因为这些函数发生在Push Model期间，导致BindAttributesToViewModels过早执行
	 */	
	virtual void SetOwnerAbilitySystemComponent(UAbilitySystemComponent* InASC);

	TObjectPtr<UBlitzAbilitySystemComponent> OwnerAbilitySystemComponent;
};  
