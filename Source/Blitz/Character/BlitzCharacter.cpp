// Copyright sumikiru. All Rights Reserved,


#include "BlitzCharacter.h"

#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"

ABlitzCharacter::ABlitzCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ABlitzCharacter::GetAbilitySystemComponent() const
{
	return GetBlitzAbilitySystemComponent();
}

void ABlitzCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

