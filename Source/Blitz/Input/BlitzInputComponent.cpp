// Copyright sumikiru. All Rights Reserved,


#include "BlitzInputComponent.h"

#include "EnhancedInputSubsystems.h"

UBlitzInputComponent::UBlitzInputComponent()
{
}

void UBlitzInputComponent::AddInputMappings(const UBlitzInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	checkf(InputConfig, TEXT("InputConfig is nullptr"));
	checkf(InputSubsystem, TEXT("InputSubsystem is nullptr"));
}

void UBlitzInputComponent::RemoveInputMappings(const UBlitzInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	checkf(InputConfig, TEXT("InputConfig is nullptr"));
	checkf(InputSubsystem, TEXT("InputSubsystem is nullptr"));
}

void UBlitzInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (const uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset(); //和Empty()不同，不会更改内存分配空间
}
