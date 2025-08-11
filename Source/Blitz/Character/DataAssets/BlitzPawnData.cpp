// Copyright sumikiru. All Rights Reserved,


#include "BlitzPawnData.h"

// https://dev.epicgames.com/documentation/zh-cn/unreal-engine/gameplay-classes-in-unreal-engine#%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0%E6%A0%BC%E5%BC%8F
UBlitzPawnData::UBlitzPawnData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CharacterClass = nullptr;
	InputConfig = nullptr;
}
