// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DataAssets/BlitzPawnData.h"
#include "BlitzCharacter.generated.h"

class UWidgetComponent;
class UBlitzPawnData;
class UBlitzAttributeSet;
class UBlitzAbilitySystemComponent;
class UInputComponent;

UENUM(BlueprintType)
enum class EWeaponEquipState : uint8
{
	Unarmed,
	Pistol,
	Rifle
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponChangedSignature, EWeaponEquipState, NewWeaponEquipState);

/**
 * The base character pawn class used by this project.
 * Responsible for sending events to pawn components.
 * New behavior should be added via pawn components when possible.
 */
UCLASS(Meta = (ShortTooltip = "The base character pawn class used by this project."))
class BLITZ_API ABlitzCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABlitzCharacter();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 拥有ASC的类，需要继承IAbilitySystemInterface并重写GetAbilitySystemComponent()方法
	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	// 由于启用了Push Model，必须手动标记脏数据并更新。只应该在Server端执行
	void SetPawnData(const UBlitzPawnData* InPawnData);

	// 判断该Character是不是该客户端玩家控制的
	bool IsLocallyControlledByPlayer() const;
	// 判断该Character是不是AI控制的（不论Server/Client）
	bool IsControlledByAI() const;

protected:
	/**
	 * 1. 对于AI控制的角色，ASC存在于Pawn中，通常在Pawn的BeginPlay()方法中完成ASC在服务器端和客户端的初始化 \n
	 * 2. 对于玩家控制的角色，如果ASC存在于Pawn中，通常在Pawn的PossessedBy()方法中完成ASC在服务器端的初始化，
	 * 在PlayerController的AcknowledgePawn()方法中完成ASC在客户端的初始化 \n
	 * 3. 对于玩家控制的角色，如果ASC存在于PlayerState中，通常在Pawn的PossessedBy()方法中完成ASC在服务器端的初始化（这一点与上述相同），
	 * 在Pawn的OnRep_PlayerState()方法中完成ASC在客户端的初始化（这将确保PlayerState在客户端已存在）。
	 */
	virtual void PossessedBy(AController* NewController) override;

	/**
	 * Enemy等非玩家角色直接在Character构造函数中实例化，Player等可能伴随重生保留ASC的角色在PlayerState的构造函数中初始化
	 * @note 不论是Enemy还是Player，在Character类中都需要创建ASC和AS。
	 * 注意PlayerState中还需要创建ASC和AS，因此后面在InitBlitzAbilityActorInfo中初始化Character时，Player从PlayerState中获取ASC和AS
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UBlitzAbilitySystemComponent> BlitzAbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UBlitzAttributeSet> BlitzAttributeSet;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> PistolMeshComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> RifleMeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName PistolUnequippedSocket = FName("PistolUnequipped");
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName RifleUnequippedSocket = FName("RifleUnequipped");

	UFUNCTION()
	void OnRep_PawnData();

	// 必须使用SetPawnData()手动标记脏数据并更新。游戏启动时需要立即为Character配置PawnData
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_PawnData, Category = "PawnData")
	TSoftObjectPtr<const UBlitzPawnData> DefaultPawnData;

	// 只能在服务端调用，且需要在InitBlitzAbilityActorInfo()之后调用（ASC实例化完成）
	void GrantPawnData();

	// 在Server端调用
	void AddInitialGameplayTags();
	
	/**=================================== UI ============================================== */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverheadWidgetComponent;
	
	// 必须在InitAbilityActorInfo之后执行（用到了ASC），且需要在Server端执行（Standalone/ListenServer）。不能直接加到BeginPlay中，ASC可能未初始化完成
	void ConfigureOverheadStatsWidget() const;
	
	/**================================ Animation =========================================== */
public:
	// 该写法不存在预测，所有端均在同一时刻LinkAnimClassLayer。如果存在延迟，本地也会在延迟后Link，从而确保强一致性
	// 而如果是GA_FireGun中这种AbilityTask，存在预测机制，客户端会预测性地播放Montage，这就和其他端不是同一时刻播放的
	// 如果Validate函数返回false,则预测错误，执行回滚操作（如立即停止正常播放的动画）
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void LinkNewAnimClassLayers(TSubclassOf<UAnimInstance> InClass, bool bLinkAnimLayer = true);
	UFUNCTION(Server, Reliable)
	void Server_LinkNewAnimClassLayers(TSubclassOf<UAnimInstance> InClass, bool bLinkAnimLayer = true);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_LinkNewAnimClassLayers(TSubclassOf<UAnimInstance> InClass, bool bLinkAnimLayer = true);
	bool NetMulticast_LinkNewAnimClassLayers_Validate(TSubclassOf<UAnimInstance> InClass, bool bLinkAnimLayer = true);

	// 由于PlayMontage需要GetAnimInstance()，因此需要为BP中的各个Mesh分配动画蓝图为AnimInstance（至少不为空）
	// PlayAnimation则不需要管这些，且仅使用PlayAnimation武器的弹匣动画会正常播放，原因未知
	// 客户端预测要使用RPC而不是RepNotify
	UFUNCTION(BlueprintCallable, Category = "Animation|Weapon")
	void PlayWeaponAnimation(UAnimationAsset* AnimToPlay);
	// 本地预测：直接播放，Validate验证。多播函数则忽略本地客户端，避免多次播放
	// 可以通过Niagara系统确定是否同步了。如果只能听到一个声音但各端都播放了特效，说明同步了，只有一个声音是因为系统只能播放当前控制端的音频
	void Local_PlayWeaponAnimation(UAnimationAsset* AnimToPlay) const;
	UFUNCTION(Server, Unreliable)
	void Server_PlayWeaponAnimation(UAnimationAsset* AnimToPlay);
	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void NetMulticast_PlayWeaponAnimation(UAnimationAsset* AnimToPlay);
	bool NetMulticast_PlayWeaponAnimation_Validate(UAnimationAsset* AnimToPlay);

	// 缓存当前Weapon，如果每次ActivateAbility时播放Montage都去获取ASC然后判断Tag，会造成大量开销。借助Delegate实现
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_CurrentEquipWeaponState, Category = "Animation")
	EWeaponEquipState CurrentEquipWeaponState = EWeaponEquipState::Pistol;
	UFUNCTION()
	void OnRep_CurrentEquipWeaponState();
	UFUNCTION(BlueprintCallable, Category = "Animation|Weapon")
	void UpdateCurrentEquipWeaponState(EWeaponEquipState NewWeaponEquipState);
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> UnarmedLayer;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> PistolLayer;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> RifleLayer;
	
private:
	// Visibility Control: 定时检测距离，距离过大则不可视
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "UI")
	float HeadStatGaugeVisibilityRangeSquared = 10000000.f;
	
	mutable FTimerHandle OverheadVisibilityUpdateTimerHandle;
	void UpdateOverheadStatsGaugeVisibility() const;
	
public:
	FORCEINLINE UBlitzAbilitySystemComponent* GetBlitzAbilitySystemComponent() const { return BlitzAbilitySystemComponent; }
	FORCEINLINE UBlitzAttributeSet* GetBlitzAttributeSet() const { return BlitzAttributeSet; }
	FORCEINLINE USkeletalMeshComponent* GetPistolMeshComponent() const { return PistolMeshComponent; }
	FORCEINLINE USkeletalMeshComponent* GetRifleMeshComponent() const { return RifleMeshComponent; }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(DefaultPawnData); }

	/** Delegates */
	UPROPERTY(BlueprintAssignable)
	FWeaponChangedSignature OnEquipWeaponStateChangedDelegate;
};
