#include "LambdaSnailActivatableWidget.h"

ULambdaSnailActivatableWidget::ULambdaSnailActivatableWidget() : Super::UCommonActivatableWidget()
{
	bIsBackHandler = true;
}

APlayerController* ULambdaSnailActivatableWidget::GetOwningPlayerController() const
{
	return GetOwningPlayer();
}

ULocalPlayer* ULambdaSnailActivatableWidget::GetLocalPlayer() const
{
	return GEngine->GetFirstLocalPlayerController(GetWorld())->GetLocalPlayer();
}
