// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "BlitzGameplayCueManager.generated.h"

/**
 * 必须继承UGameplayCueManager并告知AbilitySystemGlobals类在DefaultGame.ini中使用自己的UGameplayCueManager子类.\n
 * 在UBlitzGameplayCueManager类中, 重写ShouldAsyncLoadRuntimeObjectLibraries().
 * 只异步加载那些会在游戏中触发的GameplayCue, 这会在异步加载每个GameplayCue时减少不必要的内存占用和潜在的游戏无响应几率, 从而避免特定GameplayCue在游戏中第一次触发时可能出现的延迟效果\n
 * 详见GASDocumentation
 */
UCLASS()
class BLITZ_API UBlitzGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

public:
	/**
	 * 确实想要GameplayCueManager扫描并找到所有的GameplayCueNotify, 然而, 我们不想要它异步加载每一个,
	 * 因为这会将每个GameplayCueNotify和它们所引用的音效和粒子特效放入内存而不管它们是否在关卡中使用.
	 * 在像Paragon这样的大型游戏中, 内存中会放入数百兆的无用资源并造成卡顿和启动时无响应.\n
	 * 在启动时异步加载每个GameplayCue的一种可选方法是只异步加载那些会在游戏中触发的GameplayCue,
	 * 这会在异步加载每个GameplayCue时减少不必要的内存占用和潜在的游戏无响应几率, 从而避免特定GameplayCue在游戏中第一次触发时可能出现的延迟效果.\n
	 * 如果在UE编辑器中使用该选项并且编辑器需要编译粒子系统的话, 就可能会在GameplayCue首次加载时有轻微的卡顿或无响应, 这在构建版本中不是问题, 因为粒子系统肯定是编译好的.
	 */
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;
};
