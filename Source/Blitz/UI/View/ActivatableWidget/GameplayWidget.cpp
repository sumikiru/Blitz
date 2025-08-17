// Copyright sumikiru. All Rights Reserved,


#include "GameplayWidget.h"

#include "Blitz/BlitzLogChannels.h"

void UGameplayWidget::BindAttributesToViewModels_Implementation(UAbilitySystemComponent* InASC)
{
	SetOwnerAbilitySystemComponent(InASC);
	
	// 注意！不能通过GetOwningPlayer来获取ASC，因为前提是要SetOwningPlayer，这一步需要手动实现！
	// 不如直接SetOwnerAbilitySystemComponent()一步到位
	if (!OwnerAbilitySystemComponent)
	{
		UE_LOG(LogBlitz, Error, TEXT("OwnerAbilitySystemComponent is not valid in [%s], please check if SetOwnerAbilitySystemComponent() "
			       "is executed after BP_AddWidget(Push Widget)."), *GetNameSafe(this));
		return;
	}

	BP_BindAttributesToViewModels(OwnerAbilitySystemComponent);
}
