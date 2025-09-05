// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Blitz/UI/ViewModel/BlitzViewModelBase.h"
#include "VM_MatchState.generated.h"

/**
 * 管理比赛相关信息，如比分、倒计时
 */
UCLASS(Blueprintable, DisplayName = "Match State ViewModel")
class BLITZ_API UVM_MatchState : public UBlitzViewModelBase
{
	GENERATED_BODY()

public:
	virtual void BindPlayerController_Implementation(ABlitzPlayerController* InPlayerController) override;
	virtual void SetAndBoundToPlayerController(ABlitzPlayerController* InPlayerController);
	
	// @note: Getter/Setter不需要UFUNCTION()
	float GetCountdownTime() const;

	void SetCountdownTime(float NewCountdownTime);

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetMatchCountdownText() const;

private:
	void OnCountdownTimeChanged(const float& NewCountdownTime);
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	float CountdownTime;	// 记录的是秒数
};
