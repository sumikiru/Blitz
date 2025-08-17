// Copyright sumikiru. All Rights Reserved,


#include "VM_PercentBarBase.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"

void UVM_PercentBarBase::BindAttributes_Implementation(UBlitzAbilitySystemComponent* InASC)
{
}

void UVM_PercentBarBase::SetAndBoundToGameplayAttribute(UBlitzAbilitySystemComponent* InASC, const FGameplayAttribute& CurrentAttribute,
                                                        const FGameplayAttribute& MaxAttribute)
{
	if (!InASC)
	{
		UE_LOG(LogBlitz, Error, TEXT("InASC is not found in [%s]::SetAndBoundToGameplayAttribute()"), *GetNameSafe(this));
		return;
	}

	bool bFound = true;
	// @note: 可以直接通过ASC获取AttributeSet中相关Attribute的值（ATTRIBUTE_ACCESSORS）
	const float CurrentAttributeValue = InASC->GetGameplayAttributeValue(CurrentAttribute, bFound);
	const float MaxAttributeValue = InASC->GetGameplayAttributeValue(MaxAttribute, bFound);

	if (bFound)
	{
		SetCurrentValue(CurrentAttributeValue);
		SetMaxValue(MaxAttributeValue);
	}

	// Register for when an attribute value change
	// 现在当AS中的属性值改变时，可以通过GetGameplayAttributeValueChangeDelegate.Broadcast(NewValue)让该VM的值改变
	InASC->GetGameplayAttributeValueChangeDelegate(CurrentAttribute).AddUObject(this, &ThisClass::OnCurrentValueChanged);
	InASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &ThisClass::OnMaxValueChanged);
}

float UVM_PercentBarBase::GetCurrentValue() const
{
	return CurrentValue;
}

float UVM_PercentBarBase::GetMaxValue() const
{
	return MaxValue;
}

void UVM_PercentBarBase::SetCurrentValue(float NewCurrentValue)
{
	/**
	 * UE_MVVM_SET_PROPERTY_VALUE();内部是一个模版函数，有特化版本，最终作用是做compare并触发broadcast
	 * 原理也一样，就是通过名字传递Property做compare，如果值不变，会提前return false结束，不触发broadcast
	 * 然后通过名字拿到FieldId取Delegate进行broadcast
	 * ([Variable Name][NewValue])
	 */
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, NewCurrentValue))
	{
		// @note: 注意需要主动触发回调，告诉组件HealthPercent已经改变了，参数填函数名称
		// 如果MaxHealth改变了，HealthPercent也需要更新，内部通过名称拿到一个FieldId，然后通过FieldId找到对应的Delegate进行Broadcast
		// 直接使用UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);在Rider中会报错
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValuePercent);
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValueText);
	}
}

void UVM_PercentBarBase::SetMaxValue(float NewMaxValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxValue, NewMaxValue))
	{
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValuePercent);
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetValueText);
	}
}

float UVM_PercentBarBase::GetValuePercent() const
{
	if (MaxValue <= 0.f)
	{
		UE_LOG(LogBlitz, Warning, TEXT("Value Gauge's ViewModel: [%s], NewMaxValue must be greater than 0.f"), *GetNameSafe(this));
		return 0.f;
	}

	// AttributeSet中Health经过了Clamp，不会大于MaxHealth
	const float NewPercent = CurrentValue / MaxValue;
	return NewPercent;
}

FText UVM_PercentBarBase::GetValueText() const
{
	const FNumberFormattingOptions FormattingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	return FText::Format(
		FTextFormat::FromString("{0}/{1}"),
		FText::AsNumber(CurrentValue, &FormattingOptions),
		FText::AsNumber(MaxValue, &FormattingOptions)
	);
}

void UVM_PercentBarBase::OnCurrentValueChanged(const FOnAttributeChangeData& ChangeData)
{
	SetCurrentValue(ChangeData.NewValue);
}

void UVM_PercentBarBase::OnMaxValueChanged(const FOnAttributeChangeData& ChangeData)
{
	SetMaxValue(ChangeData.NewValue);
}
