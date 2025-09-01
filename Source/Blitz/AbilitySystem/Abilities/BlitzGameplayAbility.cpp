// Copyright sumikiru. All Rights Reserved,


#include "BlitzGameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzGameplayAbility)

UBlitzGameplayAbility::UBlitzGameplayAbility()
{
	// 默认实例化策略为InstancedPerActor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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
