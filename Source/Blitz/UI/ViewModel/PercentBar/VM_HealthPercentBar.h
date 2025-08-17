// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "VM_PercentBarBase.h"
#include "VM_HealthPercentBar.generated.h"

/**
 * Blueprintable必须写，默认的VM不支持蓝图，这样蓝图ViewModel类才能继承
 * @note 一个用户控件可以使用多个ViewModel，一个ViewModel也可以应用到多个用户控件。
 */
UCLASS(Blueprintable, DisplayName = "Health Percent Bar ViewModel")
class BLITZ_API UVM_HealthPercentBar : public UVM_PercentBarBase
{
	GENERATED_BODY()

public:
	virtual void BindAttributes_Implementation(UBlitzAbilitySystemComponent* InASC) override;
};
