// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BlitzCharacter.generated.h"

class UBlitzAttributeSet;
class UBlitzAbilitySystemComponent;
class UInputComponent;

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

	// 拥有ASC的类，需要继承IAbilitySystemInterface并重写GetAbilitySystemComponent()方法
	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

protected:
	/**
	 * 1. 对于AI控制的角色，ASC存在于Pawn中，通常在Pawn的 BeginPlay()方法中完成ASC在服务器端和客户端的初始化 \n
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
	
public:
	FORCEINLINE UBlitzAbilitySystemComponent* GetBlitzAbilitySystemComponent() const { return BlitzAbilitySystemComponent; }
	FORCEINLINE UBlitzAttributeSet* GetBlitzAttributeSet() const { return BlitzAttributeSet; }
};
