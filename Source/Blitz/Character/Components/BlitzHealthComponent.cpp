// Copyright sumikiru. All Rights Reserved,


#include "BlitzHealthComponent.h"

UBlitzHealthComponent::UBlitzHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// This should only be called during component construction
	SetIsReplicatedByDefault(true);
}

void UBlitzHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UBlitzHealthComponent* UBlitzHealthComponent::FindHealthComponent(const AActor* Actor)
{
	if (Actor)
	{
		return Actor->FindComponentByClass<UBlitzHealthComponent>();
	}
	return nullptr;
}
