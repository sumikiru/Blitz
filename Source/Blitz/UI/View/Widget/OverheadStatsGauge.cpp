// Copyright sumikiru. All Rights Reserved,


#include "OverheadStatsGauge.h"
#include "Blitz/BlitzLogChannels.h"

void UOverheadStatsGauge::BindAttributesToViewModels(UBlitzAbilitySystemComponent* InASC)
{
	if (!InASC)
	{
		UE_LOG(LogBlitz, Error, TEXT("InASC in [%s]::BindAttributes is nullptr!"), *GetNameSafe(this));
		return;
	}
	
	// 不能使用GetOwningPlayerState<ABlitzPlayerState>()->GetBlitzAbilitySystemComponent()，否则出现access violation error
	BP_BindAttributesToViewModels(InASC);
}
