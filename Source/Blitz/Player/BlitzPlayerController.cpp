// Copyright sumikiru. All Rights Reserved,


#include "BlitzPlayerController.h"

#include "BlitzPlayerState.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/UI/BlitzHUD.h"
#include "Kismet/KismetSystemLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzPlayerController)

ABlitzPlayerState* ABlitzPlayerController::GetBlitzPlayerState() const
{
	return CastChecked<ABlitzPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UBlitzAbilitySystemComponent* ABlitzPlayerController::GetBlitzAbilitySystemComponent() const
{
	const ABlitzPlayerState* BlitzPS = GetBlitzPlayerState();
	return BlitzPS ? BlitzPS->GetBlitzAbilitySystemComponent() : nullptr;
}

ABlitzHUD* ABlitzPlayerController::GetBlitzHUD() const
{
	return CastChecked<ABlitzHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

void ABlitzPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UBlitzAbilitySystemComponent* BlitzASC = GetBlitzAbilitySystemComponent())
	{
		BlitzASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ABlitzPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateMatchTime();

	CheckTimeSync(DeltaSeconds);
}

void ABlitzPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ABlitzPlayerController::Server_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	// Server接收到Client请求时的时间
	const float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds(); // 函数在Server端执行，因此获取的是Server端时间
	Client_ReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ABlitzPlayerController::Client_ReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest; // 函数在Client端执行，因此获取的是Client端时间
	const float CurrentServerTime = TimeServerReceivedClientRequest + 0.5f * RoundTripTime;

	// CurrentServerTime - CurrentClientTime;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float ABlitzPlayerController::GetServerTime()
{
	if (HasAuthority())
	{
		return GetWorld()->GetTimeSeconds();
	}
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ABlitzPlayerController::CheckTimeSync(float DeltaSeconds)
{
	// 不采用Timer，这里需要获取精确时间。Timer会导致ViewModel绑定失效，原因未知
	TimeSyncRunningTime += DeltaSeconds;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		Server_RequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

FOnMatchStateInfoChange& ABlitzPlayerController::GetMatchStateInfoChangeDelegate()
{
	return OnMatchStateCountdownTimeChangeDelegate;
}

void ABlitzPlayerController::UpdateMatchTime()
{
	const float TimeLeft = MatchTime - GetServerTime(); // 不应该使用GetWorld()->GetTimeSeconds()
	if (TimeLeft <= -1.f)
	{
		return;
	}

	// CountdownTimeInt每秒更新一次，不需要每帧更新
	const uint32 SecondsLeft = FMath::CeilToInt32(TimeLeft);

	if (CountdownTimeInt != SecondsLeft)
	{
		CountdownTimeInt = SecondsLeft;
		OnMatchStateCountdownTimeChangeDelegate.Broadcast(CountdownTimeInt);
	}
}
