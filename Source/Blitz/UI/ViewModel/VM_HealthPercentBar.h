// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzViewModelBase.h"
#include "VM_HealthPercentBar.generated.h"

/**
 * Blueprintable必须写，默认的VM不支持蓝图，这样蓝图ViewModel类才能继承
 */
UCLASS(Blueprintable, DisplayName = "Health Percent Bar ViewModel")
class BLITZ_API UVM_HealthPercentBar : public UBlitzViewModelBase
{
	GENERATED_BODY()

public:
	// @note: Getter/Setter不需要UFUNCTION()
	float GetHealth() const;
	float GetMaxHealth() const;

	void SetHealth(float NewHealth);
	void SetMaxHealth(float NewMaxHealth);

	/**
	 * 1.必须具有带 FieldNotify 和 BlueprintPure 说明符的 UFUNCTION 宏\n
	 * 2.不得带有参数\n
	 * 3.必须是 const 函数\n
	 * 4.必须仅返回单个值（没有输出参数）
	 */
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercent() const;

private:
	/**
	 * FieldNotify：使得属性可以用于通知广播，使得可以编写Getter/Setter，然后在蓝图中进行ViewBinding \n
	 * Setter:此属性可以被设置，Setter函数的名称格式 Set[Variable Name],Health的Setter为：SetHealth，注意有且只能有一个参数 \n
	 * 也可以自己指定名称Setter = [Function Name]，写法如前 \n
	 * Getter:此属性可以被获取，Getter函数的名称格式为 Get[Variable Name],Health的Getter为：GetHealth，注意不能有参数 \n
	 * 也可以自己指定名称Getter = [Function Name]，写法如前 \n
	 * 此字段在ViewModel中使用Get/Set访问，在蓝图中是Public的，在蓝图中ViewBinding使用Get/Set
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	float Health;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	float MaxHealth;
};
