// Copyright sumikiru. All Rights Reserved,


#include "BlitzHeroComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzHeroComponent)

UBlitzHeroComponent::UBlitzHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

UBlitzHeroComponent* UBlitzHeroComponent::FindHeroComponent(const AActor* Actor)
{
	if (Actor)
	{
		return Actor->FindComponentByClass<UBlitzHeroComponent>();
	}
	return nullptr;
}

void UBlitzHeroComponent::AddAdditionalInputConfig(const UBlitzInputConfig* InputConfig)
{
	
}

void UBlitzHeroComponent::RemoveAdditionalInputConfig(const UBlitzInputConfig* InputConfig)
{
}
