// Copyright sumikiru. All Rights Reserved,


#include "BlitzCommonActivatableWidget.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzCommonActivatableWidget)

void UBlitzCommonActivatableWidget::BindAttributesToViewModels_Implementation(UAbilitySystemComponent* InASC)
{
}

void UBlitzCommonActivatableWidget::SetOwnerAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
	if (!InASC)
	{
		UE_LOG(LogBlitz, Error,
		       TEXT("InASC in [%s]::SetOwnerAbilitySystemComponent() is not valid, please check if its parent can get valid local player state."),
		       *GetNameSafe(this));
		return;
	}
	OwnerAbilitySystemComponent = Cast<UBlitzAbilitySystemComponent>(InASC);
}
