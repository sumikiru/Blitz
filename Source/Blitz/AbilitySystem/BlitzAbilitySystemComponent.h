// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BlitzAbilitySystemComponent.generated.h"

/**
 * Base ability system component class used by this project.
 */
UCLASS()
class BLITZ_API UBlitzAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/** 重写了PlayerController的PostProcessInput函数，所以会Tick调用ASC上的ProcessAbilityInput函数 */
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();
	
	// Only called on the server
	void ApplyGameplayEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects);

protected:
	/**
	 * 注意需要重写这两个！
	 * 该函数正常用来处理与能力绑定的输入事件，不过Blitz输入系统接管后，就变为手动调用了。
	 * 在该函数中通过 InvokeReplicatedEvent() 通知 WaitInputPress 中的 InputPressed 事件。
	 * @note 现在需要在GA中手动Wait Input Pressed/Released然后才能执行/结束能力
	 */
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

private:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
