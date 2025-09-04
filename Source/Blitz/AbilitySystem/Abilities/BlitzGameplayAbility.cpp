// Copyright sumikiru. All Rights Reserved,


#include "BlitzGameplayAbility.h"

#include "Blitz/Character/BlitzCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzGameplayAbility)

UBlitzGameplayAbility::UBlitzGameplayAbility()
{
	// 默认实例化策略为InstancedPerActor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

EWeaponEquipState UBlitzGameplayAbility::GetAvatarActorCurrentWeaponState() const
{
	if (const ABlitzCharacter* TargetAvatarCharacter = Cast<ABlitzCharacter>(GetActorInfo().AvatarActor))
	{
		return TargetAvatarCharacter->CurrentEquipWeaponState;
	}

	UE_LOG(LogTemp, Error, TEXT("Cannot find avatar actor for ability [%s]"), *GetNameSafe(this));
	return EWeaponEquipState::Unarmed;
}

UAnimInstance* UBlitzGameplayAbility::GetOwnerAnimInstance() const
{
	if (const USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwningComponentFromActorInfo())
	{
		return OwnerSkeletalMeshComponent->GetAnimInstance();
	}

	UE_LOG(LogTemp, Warning, TEXT("No OwnerAnimInstance found for %s"), *GetNameSafe(GetOwningComponentFromActorInfo()));
	return nullptr;
}
