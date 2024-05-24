#include "LambdaSnailUiManager.h"

#include "CommonActivatableWidget.h"
#include "LambdaSnailActivatableWidget.h"
#include "LambdaSnailUILayer.h"

class UWidgetBlueprint;

void ULambdaSnailUiManager::RegisterLayer(FGameplayTag const Tag, ULambdaSnailUILayer* Layer)
{
	LayerMap.Add(Tag, Layer);
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
