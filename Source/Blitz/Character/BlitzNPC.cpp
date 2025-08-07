// Copyright sumikiru. All Rights Reserved,


#include "BlitzNPC.h"

#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/AbilitySystem/Attributes/BlitzAttributeSet.h"

ABlitzNPC::ABlitzNPC()
{
	// Enemy的ASC和AS直接在Character构造函数中实例化
	BlitzAbilitySystemComponent = CreateDefaultSubobject<UBlitzAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	BlitzAbilitySystemComponent->SetIsReplicated(true);	// 设置ASC用于在网络上复制
	// 和Player（Mixed）不同，Enemy使用Minimal
	BlitzAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	// AttributeSet本身(可视为ASC的一部分)不直接参与网络复制，由ASC处理并将AS中的数据同步到客户端
	BlitzAttributeSet = CreateDefaultSubobject<UBlitzAttributeSet>(TEXT("WarriorAttributeSet"));
}

void ABlitzNPC::BeginPlay()
{
	Super::BeginPlay();

	BlitzAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ABlitzNPC::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// todo: Grant Pawn Data
}
