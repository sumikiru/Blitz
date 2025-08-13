// Copyright sumikiru. All Rights Reserved,


#include "VM_HealthPercentBar.h"

float UVM_HealthPercentBar::GetHealth() const
{
	return Health;
}

float UVM_HealthPercentBar::GetMaxHealth() const
{
	return MaxHealth;
}

void UVM_HealthPercentBar::SetHealth(float NewHealth)
{
	/**
	 * UE_MVVM_SET_PROPERTY_VALUE();内部是一个模版函数，有特化版本，最终作用是做compare并触发broadcast
	 * 原理也一样，就是通过名字传递Property做compare，如果值不变，会提前return false结束，不触发broadcast
	 * 然后通过名字拿到FieldId取Delegate进行broadcast
	 * ([Variable Name][NewValue])
	 */
	if (UE_MVVM_SET_PROPERTY_VALUE(Health, NewHealth))
	{
		// @note: 注意需要主动触发回调，告诉组件HealthPercent已经改变了，参数填函数名称
		// 如果MaxHealth改变了，HealthPercent也需要更新，内部通过名称拿到一个FieldId，然后通过FieldId找到对应的Delegate进行Broadcast
		// 直接使用UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);在Rider中会报错
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

void UVM_HealthPercentBar::SetMaxHealth(float NewMaxHealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	}
}

float UVM_HealthPercentBar::GetHealthPercent() const
{
	if (MaxHealth <= 0.f)
	{
		return 0.f;
	}

	// AttributeSet中Health经过了Clamp，不会大于MaxHealth
	return Health / MaxHealth;
}
