// Copyright sumikiru. All Rights Reserved,


#include "BlitzPlayerState.h"

#include "BlitzPlayerController.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/AbilitySystem/Attributes/BlitzAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzPlayerState)

ABlitzPlayerState::ABlitzPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	BlitzAbilitySystemComponent = CreateDefaultSubobject<UBlitzAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	BlitzAbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourselves, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	BlitzAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	BlitzAttributeSet = CreateDefaultSubobject<UBlitzAttributeSet>(TEXT("AttributeSet"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;
}

ABlitzPlayerController* ABlitzPlayerState::GetBlitzPlayerController() const
{
	return Cast<ABlitzPlayerController>(GetOwner());
}

UBlitzAbilitySystemComponent* ABlitzPlayerState::GetBlitzAbilitySystemComponent() const
{
	return BlitzAbilitySystemComponent;
}

UAbilitySystemComponent* ABlitzPlayerState::GetAbilitySystemComponent() const
{
	return BlitzAbilitySystemComponent;
}

UBlitzAttributeSet* ABlitzPlayerState::GetBlitzAttributeSet() const
{
	return BlitzAttributeSet;
}

void ABlitzPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	checkf(BlitzAbilitySystemComponent, TEXT("AbilitySystemComponent not found on PlayerState"));
	BlitzAbilitySystemComponent->InitAbilityActorInfo(this, GetOwner());
}
