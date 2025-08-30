// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BlitzGameplayAbility.generated.h"

/**
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EBlitzAbilityActivationPolicy : uint8
{
	// 当输入动作刚刚触发（按下Pressed）的那一刻，尝试激活技能。如一次性技能（挥剑攻击、发射火球）
	OnInputTriggered,

	// 在输入动作持续处于活动状态（如按键按住Held）的整个期间，会持续尝试激活技能。如连续生效的技能（蓄力、按住持续奔跑）
	WhileInputActive,

	// 当拥有该技能的Avatar（角色实体）被生成（Spawn）到世界中时，自动尝试激活技能。如被动技能或生成时自带的持续效果
	OnSpawn
};

/**
 * The base gameplay ability class used by this project.
 */
UCLASS(Abstract, meta = (ShortToolTip = "The base gameplay ability class used by this project."))
class BLITZ_API UBlitzGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EBlitzAbilityActivationPolicy ActivationPolicy;

public:
	FORCEINLINE EBlitzAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
};
