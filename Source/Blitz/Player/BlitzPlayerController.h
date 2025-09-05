// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlitzPlayerController.generated.h"

class ABlitzHUD;
class UBlitzAbilitySystemComponent;
class ABlitzPlayerState;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateInfoChange, const float&);

/**
 * 
 */
UCLASS(Config = Game, meta = (ShortTooltip = "The base player controller class used by this project."))
class BLITZ_API ABlitzPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerController")
	ABlitzPlayerState* GetBlitzPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerController")
	UBlitzAbilitySystemComponent* GetBlitzAbilitySystemComponent() const;
	UFUNCTION(BlueprintCallable, Category = "Blitz|PlayerController")
	ABlitzHUD* GetBlitzHUD() const;

	//~APlayerController interface
	/** 重写了PlayerController的PostProcessInput函数，所以会Tick调用ASC上的ProcessAbilityInput函数 */
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

	virtual void Tick(float DeltaSeconds) override;
	// 尽早与Server时间同步
	virtual void ReceivedPlayer() override;

	/**
	 * 同步client与Server的时间。由于进入游戏的时间不同，显示的时间也不同，这时候就需要RPC同步时间
	 */
	// 请求当前服务器时间，传递发送请求时客户端的时间(TimeOfClientRequest)
	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float TimeOfClientRequest);
	// 向客户端报告当前服务器时间，以响应Server_RequestServerTime()函数的请求
	UFUNCTION(Client, Reliable)
	void Client_ReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	virtual float GetServerTime();
	void CheckTimeSync(float DeltaSeconds);

	// Client与Server之间的延迟
	float ClientServerDelta = 0.f;
	// 每隔一段时间就会再同步一次时间
	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrequency = 5.f;
	float TimeSyncRunningTime = 0.f;
	
	FOnMatchStateInfoChange& GetMatchStateInfoChangeDelegate();

protected:
	void UpdateMatchTime();

private:
	FOnMatchStateInfoChange OnMatchStateCountdownTimeChangeDelegate;

	// 比赛总时间
	float MatchTime = 30.f;
	// 比赛倒计时（剩余时间）
	uint32 CountdownTimeInt = 30;

public:
	float GetMatchTime() const { return MatchTime; }
};
