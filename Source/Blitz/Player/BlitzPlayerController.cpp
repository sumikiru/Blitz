// Copyright sumikiru. All Rights Reserved,


#include "BlitzPlayerController.h"

#include "BlitzPlayerState.h"
#include "Blitz/AbilitySystem/BlitzAbilitySystemComponent.h"
#include "Blitz/UI/BlitzHUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BlitzPlayerController)

ABlitzPlayerState* ABlitzPlayerController::GetBlitzPlayerState() const
{
	return CastChecked<ABlitzPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UBlitzAbilitySystemComponent* ABlitzPlayerController::GetBlitzAbilitySystemComponent() const
{
	const ABlitzPlayerState* BlitzPS = GetBlitzPlayerState();
	return BlitzPS ? BlitzPS->GetBlitzAbilitySystemComponent() : nullptr;
}

ABlitzHUD* ABlitzPlayerController::GetBlitzHUD() const
{
	return CastChecked<ABlitzHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

void ABlitzPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UBlitzAbilitySystemComponent* BlitzASC = GetBlitzAbilitySystemComponent())
	{
		BlitzASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
