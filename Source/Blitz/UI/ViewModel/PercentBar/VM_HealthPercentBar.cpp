// Copyright sumikiru. All Rights Reserved,


#include "VM_HealthPercentBar.h"

#include "Blitz/AbilitySystem/Attributes/BlitzAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(VM_HealthPercentBar)

void UVM_HealthPercentBar::BindAttributes_Implementation(UBlitzAbilitySystemComponent* InASC)
{
	Super::BindAttributes_Implementation(InASC);
	
	SetAndBoundToGameplayAttribute(InASC, UBlitzAttributeSet::GetHealthAttribute(), UBlitzAttributeSet::GetMaxHealthAttribute());
}
