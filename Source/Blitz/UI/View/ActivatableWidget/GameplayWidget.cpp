// Copyright sumikiru. All Rights Reserved,


#include "GameplayWidget.h"

#include "Blitz/BlitzLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayWidget)

void UGameplayWidget::BindAttributesToViewModels_Implementation(APlayerController* InPlayerController)
{
	SetOwnerInformation(InPlayerController);
	
	// 注意！不能通过GetOwningPlayer来获取ASC，因为前提是要SetOwningPlayer，这一步需要手动实现！
	// 不如直接SetOwnerInformation()一步到位
	if (!OwnerAbilitySystemComponent)
	{
		UE_LOG(LogBlitz, Error, TEXT("OwnerAbilitySystemComponent is not valid in [%s], please check if SetOwnerInformation() "
			       "is executed after BP_AddWidget(Push Widget)."), *GetNameSafe(this));
		return;
	}

	BP_BindAttributesToViewModels(OwnerAbilitySystemComponent);

	if (!OwnerPlayerController)
	{
		UE_LOG(LogBlitz, Error, TEXT("OwnerPlayerController is not valid in [%s], please check if SetOwnerInformation() "
				   "is executed after BP_AddWidget(Push Widget)."), *GetNameSafe(this));
		return;
	}
	BP_BindPlayerControllerToViewModels(OwnerPlayerController);
}
