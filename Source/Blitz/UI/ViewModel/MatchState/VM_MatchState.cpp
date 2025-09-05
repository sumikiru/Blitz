// Copyright sumikiru. All Rights Reserved,


#include "VM_MatchState.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/Player/BlitzPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(VM_MatchState)

void UVM_MatchState::BindPlayerController_Implementation(ABlitzPlayerController* InPlayerController)
{
	Super::BindPlayerController_Implementation(InPlayerController);

	SetAndBoundToPlayerController(InPlayerController);
}

void UVM_MatchState::SetAndBoundToPlayerController(ABlitzPlayerController* InPlayerController)
{
	if (!InPlayerController)
	{
		UE_LOG(LogBlitz, Error, TEXT("InPlayerController is not found in [%s]::SetAndBoundToPlayerController()"), *GetNameSafe(this));
		return;
	}

	// 注意初始化
	const float InitialMatchCountdownTime = InPlayerController->GetMatchTime();
	SetCountdownTime(InitialMatchCountdownTime);

	InPlayerController->GetMatchStateInfoChangeDelegate().AddUObject(this, &ThisClass::OnCountdownTimeChanged);
}

float UVM_MatchState::GetCountdownTime() const
{
	return CountdownTime;
}

void UVM_MatchState::SetCountdownTime(float NewCountdownTime)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CountdownTime, NewCountdownTime))
	{
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCountdownTime);
		FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMatchCountdownText);
	}
}

FText UVM_MatchState::GetMatchCountdownText() const
{
	const int32 Minutes = FMath::FloorToInt32(CountdownTime / 60.f);
	const int32 Seconds = CountdownTime - Minutes * 60;

	// %02d: 数字格式化为两位数，左侧填充0。比如8格式化为08
	return FText::FromString(
		FString::Printf(TEXT("%02d：%02d"), Minutes, Seconds)
	);
}

void UVM_MatchState::OnCountdownTimeChanged(const float& NewCountdownTime)
{
	SetCountdownTime(NewCountdownTime);
}
