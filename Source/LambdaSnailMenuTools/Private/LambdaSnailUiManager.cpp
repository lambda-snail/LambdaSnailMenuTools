#include "LambdaSnailUiManager.h"

#include "CommonActivatableWidget.h"
#include "LambdaSnailActivatableWidget.h"
#include "LambdaSnailUILayer.h"
#include "Input/CommonUIActionRouterBase.h"

class UWidgetBlueprint;

void ULambdaSnailUiManager::RegisterLayer(FLayerRegistrationParams LayerRegistrationParams)
{
	FLayerContainer Layer
	{
		.Layer = LayerRegistrationParams.Layer,
		.bIsBackgroundLayer = LayerRegistrationParams.bIsBackgroundLayer,
		.OnWidgetChangeDelegateHandle =
			LayerRegistrationParams.Layer->OnDisplayedWidgetChanged().AddUObject(this, &ULambdaSnailUiManager::WidgetContainer_OnDisplayedWidgetChanged)
	};

	LayerMap.Add(LayerRegistrationParams.Tag, Layer);
}

void ULambdaSnailUiManager::PushWidgetToLayer(FGameplayTag const& LayerTag, TSubclassOf<ULambdaSnailActivatableWidget> WidgetClass)
{
	if(FLayerContainer const* Layer = LayerMap.Find(LayerTag))
	{
		Layer->Layer->AddWidget<ULambdaSnailActivatableWidget>(WidgetClass);
	}
}

void ULambdaSnailUiManager::PopWidgetFromLayer(FGameplayTag const LayerTag)
{
	if(FLayerContainer const* Layer = LayerMap.Find(LayerTag))
	{
		UCommonActivatableWidget* Widget = Layer->Layer->GetActiveWidget();
		Widget->DeactivateWidget();
	}
}

void ULambdaSnailUiManager::NativeDestruct()
{
	for (auto [Tag, Layer] : LayerMap)
	{
		Layer.OnWidgetChangeDelegateHandle.Reset();
	}

	LayerMap.Empty();
	Super::NativeDestruct();
}

/**
 * When a widget is added or removed, check if there are any active widgets left that are not
 * marked as background widgets, and restore input to the game. This is necessary as CommonUI
 * does not restore the input in this case.
 */
void ULambdaSnailUiManager::WidgetContainer_OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget)
{
	bool bHasActiveWidgets = false;
	for (auto [Tag, Layer] : LayerMap)
	{
		if(Layer.Layer->GetActiveWidget() and not Layer.bIsBackgroundLayer)
		{
			bHasActiveWidgets = true;
			break;
		}
	}

	// Active widgets exist, leave input to CommonUI
	if(bHasActiveWidgets)
	{
		return;
	}

	// No active widgets, restore input to game
	ULocalPlayer const* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(UCommonUIActionRouterBase* InputSubsystem = LocalPlayer->GetSubsystem<UCommonUIActionRouterBase>())
	{
		InputSubsystem->SetActiveUIInputConfig(DefaultInputConfig);
	}
}
