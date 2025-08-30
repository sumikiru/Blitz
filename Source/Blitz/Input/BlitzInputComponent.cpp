// Copyright sumikiru. All Rights Reserved,


#include "BlitzInputComponent.h"

#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzInputComponent)

void UBlitzInputComponent::AddInputMappingContexts(const UBlitzInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	checkf(InputConfig, TEXT("InputConfig is nullptr"));
	checkf(InputSubsystem, TEXT("InputSubsystem is nullptr"));

	for (const FBlitzInputMappingContextAndPriority& Mapping : InputConfig->InputMappingContextAndPriorities)
	{
		if (const UInputMappingContext* IMC = Mapping.InputMappingContext)
		{
			FModifyContextOptions Options{};
			Options.bIgnoreAllPressedKeysUntilRelease = false;
			InputSubsystem->AddMappingContext(IMC, Mapping.Priority, Options);
		}
	}
}

void UBlitzInputComponent::RemoveInputMappingContexts(const UBlitzInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	checkf(InputConfig, TEXT("InputConfig is nullptr"));
	checkf(InputSubsystem, TEXT("InputSubsystem is nullptr"));

	for (const FBlitzInputMappingContextAndPriority& Mapping : InputConfig->InputMappingContextAndPriorities)
	{
		if (const UInputMappingContext* IMC = Mapping.InputMappingContext)
		{
			InputSubsystem->RemoveMappingContext(IMC);
		}
	}
}

void UBlitzInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (const uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset(); //和Empty()不同，不会更改内存分配空间
}
