// Copyright sumikiru. All Rights Reserved,


#include "BlitzCharacter.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/UI/View/Widget/OverheadStatsGauge.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABlitzCharacter::ABlitzCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Overhead Stats Gauge显示在玩家/NPC头顶，应作为组件，同时这样也方便显示每个人的属性
	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Over Head Widget Component"));
	OverheadWidgetComponent->SetupAttachment(GetRootComponent());
}

void ABlitzCharacter::BeginPlay()
{
	// 所有网络模式均会执行（含单机Standalone、监听服务器Listen Server、客户端），BeginPlay()在游戏开始/玩家生成（含重生）时会调用
	Super::BeginPlay();
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
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, HeadStatGaugeVisibilityCheckUpdateGap, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, HeadStatGaugeVisibilityRangeSquared, SharedParams);
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
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DefaultPawnData, this); // 仅能在Server端执行
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

bool ABlitzCharacter::IsLocallyControlledByPlayer() const
{
	// 使用GetController() != nullptr判断的依据是：每个客户端只有自己的PlayerController以及其他所有Character，没有Server端的PlayerController，也没有AIController
	// 但是GetController() != nullptr在Dedicated Server上不起作用
	// 因此还需要GetController()->IsLocalPlayerController()判断是否本地玩家控制(Client)
	return GetController() && GetController()->IsLocalPlayerController();
}

bool ABlitzCharacter::IsControlledByAI() const
{
	// 和IsBotControlled()的区别，该函数仅仅区分玩家/AI
	return GetController() && !GetController()->IsPlayerController();
}

// only called on server
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

void ABlitzCharacter::GrantPawnData()
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

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, HeadStatGaugeVisibilityCheckUpdateGap, this);
		HeadStatGaugeVisibilityCheckUpdateGap = LoadedData->HeadStatGaugeVisibilityCheckUpdateGap;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, HeadStatGaugeVisibilityRangeSquared, this);
		HeadStatGaugeVisibilityRangeSquared = LoadedData->HeadStatGaugeVisibilityRangeSquared;
	}
}

void ABlitzCharacter::ConfigureOverheadStatsWidget() const
{
	// IsLocallyControlled()包括了Listen Server和Client。但是这里不能加判断，因为每个Client/Listen Server需要为每个Character都配置
	// 但不是每个Character都是Locally Controlled
	/*if (!IsLocallyControlled())
	{
		return;
	}*/

	if (!OverheadWidgetComponent)
	{
		UE_LOG(LogBlitz, Error, TEXT("Overhead Widget Component is not valid on player character [%s]"), *GetNameSafe(this));
		return;
	}

	// 本地控制者（包括监听服务器上的本地玩家）隐藏自己的血条（仅玩家自己的界面）
	if (IsLocallyControlledByPlayer())
	{
		UE_LOG(LogBlitz, Display, TEXT("隐藏：[%s]的OverheadWidgetComponent"), *GetNameSafe(this));
		OverheadWidgetComponent->SetHiddenInGame(true);
		return; // 别忘了！
	}

	if (UOverheadStatsGauge* OverheadStatsGauge = Cast<UOverheadStatsGauge>(OverheadWidgetComponent->GetUserWidgetObject()))
	{
		if (!BlitzAbilitySystemComponent)
		{
			UE_LOG(LogBlitz, Error, TEXT("BlitzASC is not valid on character [%s], please check function call time"), *GetNameSafe(this));
			return;
		}
		OverheadStatsGauge->BindAttributesToViewModels(BlitzAbilitySystemComponent);
		OverheadWidgetComponent->SetHiddenInGame(false); // 其他玩家显示

		// const函数，设置OverheadVisibilityUpdateTimerHandle为mutable
		GetWorldTimerManager().ClearTimer(OverheadVisibilityUpdateTimerHandle);
		GetWorldTimerManager().SetTimer(
			OverheadVisibilityUpdateTimerHandle,
			this,
			&ThisClass::UpdateOverheadStatsGaugeVisibility,
			HeadStatGaugeVisibilityCheckUpdateGap,
			true
		);
	}
	else
	{
		UE_LOG(LogBlitz, Error, TEXT("OverheadStatsGauge is not valid on character [%s]"), *GetNameSafe(this));
	}
}

void ABlitzCharacter::UpdateOverheadStatsGaugeVisibility() const
{
	// PlayerIndex0: LocalPlayer
	if (const APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		// 该Character与LocalPlayerPawn的距离平方
		const float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		const bool bShouldHideWidgetComponent = DistSquared > HeadStatGaugeVisibilityRangeSquared;
		
		OverheadWidgetComponent->SetHiddenInGame(bShouldHideWidgetComponent);
	}
	else
	{
		UE_LOG(LogBlitz, Warning, TEXT("Cannot find local player pawn(PlayerIndex 0)"));
	}
}
