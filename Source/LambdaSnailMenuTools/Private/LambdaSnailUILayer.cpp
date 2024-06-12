#include "LambdaSnailUILayer.h"
#include "LambdaSnailActivatableWidget.h"

void ULambdaSnailUILayer::Initialize_Implementation()
{
	for(const auto& [WidgetClass] : DefaultWidgets)
	{
		AddWidget<ULambdaSnailActivatableWidget>(WidgetClass);
	}
}
