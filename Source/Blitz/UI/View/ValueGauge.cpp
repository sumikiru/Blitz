// Copyright sumikiru. All Rights Reserved,


#include "ValueGauge.h"

#include "Blitz/BlitzLogChannels.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();

	ensureMsgf(ProgressBar, TEXT("ProgressBar [%s] is not set in ValueGauge: [%s]"), *GetNameSafe(ProgressBar), *GetNameSafe(this));
	if (ProgressBar)
	{
		ProgressBar->SetFillColorAndOpacity(BarColor);
	}
}

void UValueGauge::SetValue(float NewValue, float NewMaxValue)
{
	if (NewMaxValue <= 0.f)
	{
		UE_LOG(LogBlitz, Warning, TEXT("Value Gauge: [%s], NewMaxValue must be greater than 0.f"), *GetNameSafe(this));
		return;
	}

	const float NewPercent = NewValue / NewMaxValue;
	ProgressBar->SetPercent(NewPercent);

	const FNumberFormattingOptions FormattingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(NewValue, &FormattingOptions),
			FText::AsNumber(NewMaxValue, &FormattingOptions)
		)
	);
}
