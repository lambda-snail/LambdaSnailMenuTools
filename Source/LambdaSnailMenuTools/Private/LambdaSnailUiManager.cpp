#include "LambdaSnailUiManager.h"

#include "CommonActivatableWidget.h"
#include "LambdaSnailActivatableWidget.h"
#include "LambdaSnailUILayer.h"
#include "Input/CommonUIActionRouterBase.h"

class UWidgetBlueprint;

void ULambdaSnailUiManager::RegisterLayer(FGameplayTag const Tag, ULambdaSnailUILayer* Layer)
{
	LayerMap.Add(Tag, Layer);
	FDelegateHandle Handle = Layer->OnDisplayedWidgetChanged().AddUObject(this, &ULambdaSnailUiManager::WidgetContainer_OnDisplayedWidgetChanged);
	LayerCallbackHandleMap.Add(Tag, Handle);
}

void ULambdaSnailUiManager::PushWidgetToLayer(FGameplayTag const LayerTag, TSubclassOf<ULambdaSnailActivatableWidget> WidgetClass)
{
	if(TObjectPtr<ULambdaSnailUILayer> const* Layer = LayerMap.Find(LayerTag))
	{
		Layer->Get()->AddWidget<ULambdaSnailActivatableWidget>(WidgetClass);
	}
}

void ULambdaSnailUiManager::PopWidgetFromLayer(FGameplayTag const LayerTag)
{
	if(TObjectPtr<ULambdaSnailUILayer> const* Layer = LayerMap.Find(LayerTag))
	{
		UCommonActivatableWidget* Widget = Layer->Get()->GetActiveWidget();
		Widget->DeactivateWidget();
	}
}

void ULambdaSnailUiManager::NativeDestruct()
{
	for (auto [Tag, Handle] : LayerCallbackHandleMap)
	{
		Handle.Reset();
	}

	LayerCallbackHandleMap.Empty();
	Super::NativeDestruct();
}

void ULambdaSnailUiManager::WidgetContainer_OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget)
{
	bool bHasActiveWidgets = false;
	for (auto [Tag, Layer] : LayerMap)
	{
		if(Layer->GetActiveWidget())
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
		FUIInputConfig const InputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently);
		InputSubsystem->SetActiveUIInputConfig(InputConfig);
	}
}

//
// void ULambdaSnailUiManager::SetInputMode(EInputMode InputMode, APlayerController* PlayerController) const
// {
// 	switch(InputMode)
// 	{
// 	case EInputMode::GameOnly:
// 		{
// 			FInputModeGameOnly InputModeData{};
// 			PlayerController->SetInputMode(InputModeData);
// 		}
// 		break;
// 	case EInputMode::UIOnly:
// 		{
// 			FInputModeUIOnly InputModeData{};
// 			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
// 			PlayerController->SetInputMode(InputModeData);
// 		}
// 		break;
// 	case EInputMode::GameAndUI:
// 		{
// 			FInputModeGameAndUI InputModeData{};
// 			PlayerController->SetInputMode(InputModeData);
// 		}
// 		break;
// 	default:
// 		UE_LOGFMT(LogTemp, Error, "Unknown input mode: {InputMode}", static_cast<int32>(InputMode));
// 	}
// }
