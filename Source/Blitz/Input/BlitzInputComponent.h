// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "BlitzInputConfig.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "BlitzInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UBlitzInputConfig;

/**
 * Component used to manage input mappings and bindings using an input config data asset(Config = Input).
 * 配置存在DefaultInput.ini中
 * 可以参考https://zhuanlan.zhihu.com/p/611735265
 */
UCLASS()
class BLITZ_API UBlitzInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// InputMappingContext
	void AddInputMappingContexts(const UBlitzInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappingContexts(const UBlitzInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	// InputAction
	template <class UserClass, typename FuncType>
	void BindNativeAction(const UBlitzInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func,
	                      bool bLogIfNotFound = false);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UBlitzInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
	                        TArray<uint32>& BindHandles);

	// 将InputAction与对应事件解绑
	void RemoveBinds(TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UBlitzInputComponent::BindNativeAction(const UBlitzInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object,
                                            FuncType Func, bool bLogIfNotFound)
{
	checkf(InputConfig, TEXT("InputConfig [%s] is not set!"), *GetNameSafe(InputConfig));

	if (const UInputAction* InputAction = InputConfig->FindNativeInputActionByTag(InputTag, bLogIfNotFound))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

// 在AbilitySet中为GA绑定InputTag
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UBlitzInputComponent::BindAbilityActions(const UBlitzInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	checkf(InputConfig, TEXT("InputConfig [%s] is not set!"), *GetNameSafe(InputConfig));

	for (const FBlitzInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				// BindAction会返回FEnhancedInputActionEventBinding结构体，其父类FInputBindingHandle中存在uint32类型的Handle
				// 将Handle记录到BindHandles中，用于在RemoveBinds中解绑
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}
			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
