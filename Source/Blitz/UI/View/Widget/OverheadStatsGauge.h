// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "OverHeadStatsGauge.generated.h"

class UBlitzAbilitySystemComponent;
class UValueGauge;
/**
 * 敌人/队友头上的信息
 */
UCLASS()
class BLITZ_API UOverheadStatsGauge : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	// 参考GameplayWidget
	void BindAttributesToViewModels(UBlitzAbilitySystemComponent* InASC);
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|ViewModel")
	void BP_BindAttributesToViewModels(UBlitzAbilitySystemComponent* InASC);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UValueGauge> OverheadHealthBar;	// 应使用WBP_ValueGauge
};
