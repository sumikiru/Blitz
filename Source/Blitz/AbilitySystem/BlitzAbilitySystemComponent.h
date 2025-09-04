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

	/**
	 * 客户端GameplayCue
	 * 由于从GameplayAbility和ASC中暴露的用于触发GameplayCue的函数默认是可同步的. 每个GameplayCue事件都是一个多播(Multicast)RPC. 这会导致大量RPC.
	 * GAS也强制在每次网络更新中最多能有**两个**相同的GameplayCueRPC.
	 * 我们可以通过使用客户端GameplayCue来避免这个问题. 客户端GameplayCue只能在单独的客户端上Execute, Add或Remove.\n
	 * 如果某个GameplayCue是客户端添加的, 那么它也应该自客户端移除. 如果它是通过同步添加的, 那么它也应该通过同步移除.\n
	 * 使用客户端GameplayCue的场景：抛射物伤害、近战碰撞伤害、动画蒙太奇触发的GameplayCue
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

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
