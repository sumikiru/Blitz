// Copyright sumikiru. All Rights Reserved,


#include "BlitzAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UBlitzAttributeSet::UBlitzAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);

	bOutOfHealth = false;
	MaxHealthBeforeAttributeChange = 0.f;
	HealthBeforeAttributeChange = 0.f;
}

UWorld* UBlitzAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UBlitzAbilitySystemComponent* UBlitzAttributeSet::GetBlitzAbilitySystemComponent() const
{
	return Cast<UBlitzAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

void UBlitzAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBlitzAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBlitzAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UBlitzAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlitzAttributeSet, Health, OldHealth);

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldHealth.GetCurrentValue();

	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldHealth.GetCurrentValue(), CurrentHealth);

	if (!bOutOfHealth && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldHealth.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = (CurrentHealth <= 0.f);
}

void UBlitzAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlitzAttributeSet, MaxHealth, OldMaxHealth);

	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxHealth() - OldMaxHealth.GetCurrentValue(), OldMaxHealth.GetCurrentValue(), GetMaxHealth());
}

bool UBlitzAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// Save the current health before any changes
	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();

	return true;
}

void UBlitzAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.f;
	
#if !UE_BUILD_SHIPPING
	// todo: cheat mode->set MinimumHealth to 1.f
#endif

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp and fall into out of health handling below(health has changed, use SetHealth() just to clamp its value)
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
		// DO NOT broadcast immediately, check if health has actually changed activate callbacks as below
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		// Notify on any requested max health changes
		OnMaxHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxHealthBeforeAttributeChange, GetMaxHealth());
	}

	/** If health has actually changed activate callbacks */
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		OnHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}

	if (GetHealth() <= 0.f && !bOutOfHealth)
	{
		OnOutOfHealth.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}

	// Check health again in case an event above changed it.
	bOutOfHealth = (GetHealth() <= 0.f);
}

void UBlitzAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UBlitzAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UBlitzAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetHealth() > NewValue)
		{
			UBlitzAbilitySystemComponent* BlitzASC = GetBlitzAbilitySystemComponent();
			check(BlitzASC);

			/**
			 * @note: use ApplyModToAttribute() to override HealthAttribute instead of calling SetHealth(), See the comments for details
			 * 调用SetHealth()本质上是使用SetNumericAttributeBase，
			 * SetNumericAttributeBase是直接设置属性的数值，而ApplyModToAttribute是对属性的基础值进行数学运算，得到新的属性值。
			 * 其底层也是调用 SetNumericAttributeBase 的，只是在调用之前目标值是经公式计算得来的
			 * @note: 目标属性的增减应该用 ApplyModToAttribute 而不是 SetNumericAttributeBase ，否则在技能即可cost某属性又可增加该属性时，可能导致该属性增减异常。
			 * 通常在PostGameplayEffectExecute中（通过GE Spec）使用SetHealth，PostAttributeChange中使用ApplyModToAttribute()
			 */
			BlitzASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfHealth && GetHealth() > 0.f)
	{
		bOutOfHealth = false;
	}
}

void UBlitzAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.f);
	}
}
