// Copyright sumikiru. All Rights Reserved,


#include "AN_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Blitz/BlitzLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AN_SendGameplayEvent)

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find Mesh Comp [%s] owner"), *GetNameSafe(MeshComp));
		return;
	}
	if (!EventTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Event Tag [%s]!"), *EventTag.ToString());
		return;
	}

	const UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (!OwnerASC)
	{
		UE_LOG(LogBlitzAbilitySystem, Error, TEXT("Invalid ASC in owner [%s]"), *GetNameSafe(MeshComp->GetOwner()));
		return;
	}

	// 后续其他GA中采用UAbilityTask_WaitGameplayEvent
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		MeshComp->GetOwner(),
		EventTag,
		FGameplayEventData()	/** 注意暂未填写信息 */
	);
}

FString UAN_SendGameplayEvent::GetNotifyName_Implementation() const
{
	if (!EventTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Event Tag [%s]!"), *EventTag.ToString());
		return FString("None");
	}

	/*TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	return TagNames.Last().ToString();*/
	return EventTag.ToString();
}
