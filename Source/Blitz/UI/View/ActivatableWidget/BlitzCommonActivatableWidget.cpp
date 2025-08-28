// Copyright sumikiru. All Rights Reserved,


#include "BlitzCommonActivatableWidget.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzCommonActivatableWidget)

TOptional<FUIInputConfig> UBlitzCommonActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EBlitzWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EBlitzWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EBlitzWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EBlitzWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}

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
