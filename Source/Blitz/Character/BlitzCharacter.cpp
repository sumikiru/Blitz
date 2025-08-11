// Copyright sumikiru. All Rights Reserved,


#include "BlitzCharacter.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

ABlitzCharacter::ABlitzCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABlitzCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/**
	 * bIsPushBased = true启用Push Model后，注意：需在属性修改时调用MARK_PROPERTY_DIRTY显式标记脏状态
	 * 引擎仅检查被标记的属性，大幅减少比较次数。开销较小，适用于低频变化的属性（如配置类资产引用）
	 */
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DefaultPawnData, SharedParams);
}

UAbilitySystemComponent* ABlitzCharacter::GetAbilitySystemComponent() const
{
	return GetBlitzAbilitySystemComponent();
}

void ABlitzCharacter::SetPawnData(const UBlitzPawnData* InPawnData)
{
	checkf(InPawnData, TEXT("In pawn data is not valid!"));

	if (!HasAuthority())
	{
		return;
	}

	if (DefaultPawnData)
	{
		UE_LOG(LogBlitz, Error, TEXT("Trying to set PawnData [%s] on player character [%s] that already has valid PawnData [%s]."),
		       *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(DefaultPawnData.Get()));
		return;
	}

	// @note: 由于启用了Push Model，需在属性修改时调用MARK_PROPERTY_DIRTY显式标记脏状态。先标记为脏，再修改
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DefaultPawnData, this);	// 仅能在Server端执行
	DefaultPawnData = InPawnData;

	/*
	for (const UBlitzAbilitySet* AbilitySet : DefaultPawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(BlitzAbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_LyraAbilityReady);
	*/

	ForceNetUpdate(); // note
}

void ABlitzCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

/**
 * OnRep_PawnData()函数是仅在客户端触发的回调函数，同步完成后，在客户端自动调用
 */
void ABlitzCharacter::OnRep_PawnData()
{
	UE_LOG(LogBlitz, Log, TEXT("Pawn Data has been synced"));
}

void ABlitzCharacter::GrantPawnData() const
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (DefaultPawnData.IsNull())
	{
		UE_LOG(LogBlitz, Warning, TEXT("Default Pawn Data [%s] on player character [%s] is null."),
		       *GetNameSafe(DefaultPawnData.Get()), *GetNameSafe(this));
		return;
	}

	if (const UBlitzPawnData* LoadedData = DefaultPawnData.LoadSynchronous())
	{
		BlitzAbilitySystemComponent->ApplyGameplayEffects(LoadedData->InitialEffects);
	}
}
