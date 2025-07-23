// Copyright sumikiru. All Rights Reserved,


#include "BlitzCharacter.h"

ABlitzCharacter::ABlitzCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABlitzCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

