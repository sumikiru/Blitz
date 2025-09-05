// Copyright sumikiru. All Rights Reserved,


#include "LagCompensationComponent.h"

#include "Blitz/BlitzLogChannels.h"
#include "Blitz/Character/BlitzCharacter.h"
#include "Components/BoxComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LagCompensationComponent)

ULagCompensationComponent::ULagCompensationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();

	FFramePackage Package;
	SaveFramePackage(Package);
	ShowDebugFramePackage(Package);
}

void ULagCompensationComponent::SaveFramePackage(FFramePackage& Package) const
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogBlitz, Error, TEXT("Invalid owner character of lag compensation component [%s]"), *GetNameSafe(this));
		return;
	}

	// 这里的Time是服务器时间。需要将权威服务器的世界时间同步给其他的普通客户端，而中途加入的玩家也应该进行同步，
	// 所以我们在玩家加入房间的时候同步时间，可以重写APlayerController::ReceivedPlayer
	Package.Time = GetWorld()->GetTimeSeconds();

	for (auto& [BoxName, HitBox] : OwnerCharacter->HitCollisionBoxes)
	{
		FBoxInformation BoxInformation;
		BoxInformation.Location = HitBox->GetComponentLocation();
		BoxInformation.Rotation = HitBox->GetComponentRotation();
		BoxInformation.BoxExtent = HitBox->GetScaledBoxExtent();

		Package.HitBoxInfo.Add(BoxName, BoxInformation);
	}
}

void ULagCompensationComponent::ShowDebugFramePackage(const FFramePackage& Package, const FColor& Color) const
{
	for (auto& [BoxName, BoxInformation] : Package.HitBoxInfo)
	{
		DrawDebugBox(
			GetWorld(),
			BoxInformation.Location,
			BoxInformation.BoxExtent,
			FQuat(BoxInformation.Rotation),
			Color,
			true
		);
	}
}
