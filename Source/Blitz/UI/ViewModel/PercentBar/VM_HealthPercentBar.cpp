// Copyright sumikiru. All Rights Reserved,


#include "VM_HealthPercentBar.h"

#include "Blitz/AbilitySystem/Attributes/BlitzAttributeSet.h"

void UVM_HealthPercentBar::BindAttributes_Implementation(UBlitzAbilitySystemComponent* InASC)
{
	Super::BindAttributes_Implementation(InASC);
	
	SetAndBoundToGameplayAttribute(InASC, UBlitzAttributeSet::GetHealthAttribute(), UBlitzAttributeSet::GetMaxHealthAttribute());
}
