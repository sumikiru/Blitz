// Copyright sumikiru. All Rights Reserved,


#include "BlitzCommonActivatableWidget.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/Player/BlitzPlayerController.h"

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

void UBlitzCommonActivatableWidget::BindAttributesToViewModels_Implementation(APlayerController* InPlayerController)
{
}

void UBlitzCommonActivatableWidget::SetOwnerInformation(APlayerController* InController)
{
	if (!InController)
	{
		UE_LOG(LogBlitz, Error,
		       TEXT("InController in [%s]::SetOwnerAbilitySystemComponent() is not valid, please check if its parent can get valid local player controller."),
		       *GetNameSafe(this));
		return;
	}
	OwnerPlayerController = Cast<ABlitzPlayerController>(InController);

	// ASC在PlayerState上，可以通过PlayerState获取，无法直接通过PlayerController获取，但可以根据PlayerController获取PlayerState
	// 详见ABlitzPlayerController::GetBlitzAbilitySystemComponent()
	OwnerAbilitySystemComponent = OwnerPlayerController->GetBlitzAbilitySystemComponent();
}
