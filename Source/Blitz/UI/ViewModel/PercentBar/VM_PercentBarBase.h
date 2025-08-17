// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Blitz/UI/ViewModel/BlitzViewModelBase.h"
#include "VM_PercentBarBase.generated.h"

struct FOnAttributeChangeData;
class UBlitzAbilitySystemComponent;
struct FGameplayAttribute;
/**
 * @note 一个用户控件可以使用多个ViewModel，一个ViewModel也可以应用到多个用户控件。
 */
UCLASS(Abstract)
class BLITZ_API UVM_PercentBarBase : public UBlitzViewModelBase
{
	GENERATED_BODY()

public:
	// 抽象类不能有UFUNCTION(BlueprintCallable, Category = "UI|Player")
	virtual void BindAttributes_Implementation(UBlitzAbilitySystemComponent* InASC) override;
	
	void SetAndBoundToGameplayAttribute(UBlitzAbilitySystemComponent* InASC, const FGameplayAttribute& CurrentAttribute,
	                                            const FGameplayAttribute& MaxAttribute);

	// @note: Getter/Setter不需要UFUNCTION()
	float GetCurrentValue() const;
	float GetMaxValue() const;

	void SetCurrentValue(float NewCurrentValue);
	void SetMaxValue(float NewMaxValue);

	/**
	 * 1.必须具有带 FieldNotify 和 BlueprintPure 说明符的 UFUNCTION 宏\n
	 * 2.不得带有参数\n
	 * 3.必须是 const 函数\n
	 * 4.必须仅返回单个值（没有输出参数）
	 */
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetValuePercent() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetValueText() const;

private:
	void OnCurrentValueChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxValueChanged(const FOnAttributeChangeData& ChangeData);
	
	/**
	 * FieldNotify：使得属性可以用于通知广播，使得可以编写Getter/Setter，然后在蓝图中进行ViewBinding \n
	 * Setter:此属性可以被设置，Setter函数的名称格式 Set[Variable Name],Health的Setter为：SetHealth，注意有且只能有一个参数 \n
	 * 也可以自己指定名称Setter = [Function Name]，写法如前 \n
	 * Getter:此属性可以被获取，Getter函数的名称格式为 Get[Variable Name],Health的Getter为：GetHealth，注意不能有参数 \n
	 * 也可以自己指定名称Getter = [Function Name]，写法如前 \n
	 * 此字段在ViewModel中使用Get/Set访问，在蓝图中是Public的，在蓝图中ViewBinding使用Get/Set
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	float CurrentValue = 100.f;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	float MaxValue = 180.f;	
};
