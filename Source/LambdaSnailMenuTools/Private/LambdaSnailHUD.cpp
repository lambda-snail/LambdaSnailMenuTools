#include "LambdaSnailHUD.h"

#include "LambdaSnailUILayer.h"
#include "Blueprint/UserWidget.h"
#include "MenuTags.h"
#include "Logging/StructuredLog.h"

void ALambdaSnailHUD::RegisterLayer(FGameplayTag const LayerTag, ULambdaSnailUILayer* Layer)
{
	LayerMap.Add(LayerTag, Layer);
	Layer->AddToViewport();
}

void ALambdaSnailHUD::PushScreenToLayer(FGameplayTag const LayerTag, FGameplayTag const ScreenTag)
{
	if(TObjectPtr<ULambdaSnailUILayer> const* Layer = LayerMap.Find(LayerTag))
	{
		(*Layer)->PushScreen(ScreenTag);
	}
}

void ALambdaSnailHUD::PopScreenFromLayer(FGameplayTag const LayerTag)
{
	if(TObjectPtr<ULambdaSnailUILayer> const* Layer = LayerMap.Find(LayerTag))
	{
		(*Layer)->PopScreen();
	}
}

// void ALambdaSnailHUD::CreateScreenPtr(APlayerController* Controller, const FScreenParams& WidgetDefinition, FScreenPtr& OutScreen) const
// {
// 	// UUserWidget* Widget = CreateWidget<UUserWidget>(Controller, WidgetDefinition.WidgetType);
// 	// Widget->AddToViewport();
// 	// Widget->SetVisibility(ESlateVisibility::Collapsed);
// 	//
// 	// OutScreen = MakeShared<FManagedScreen>();
// 	// OutScreen->WidgetTag = WidgetDefinition.WidgetTag;
// 	// OutScreen->Widget = Widget;
// 	// OutScreen->bShowMouseCursor = WidgetDefinition.bShowMouseCursor;
// 	// OutScreen->PreferredInputMode = WidgetDefinition.PreferredInputMode;
// 	// OutScreen->PreferredVisibility =  WidgetDefinition.PreferredVisibility;
// }


void ALambdaSnailHUD::CollapseAllExceptHUD()
{

}

// void ALambdaSnailHUD::SetControllerOptions(FScreenPtr const& Screen) const
// {
// 	UWorld const* World = GetWorld();
// 	if(not World)
// 	{
// 		return;
// 	}
//
// 	APlayerController* PlayerController = World->GetFirstPlayerController();
// 	if(not PlayerController)
// 	{
// 		return;
// 	}
//
// 	SetControllerOptions(Screen, PlayerController);
// }
//
// void ALambdaSnailHUD::SetInputMode(EInputMode InputMode, APlayerController* PlayerController) const
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
// 	case EInputMode::UIAndGame:
// 		{
// 			FInputModeGameAndUI InputModeData{};
// 			PlayerController->SetInputMode(InputModeData);
// 		}
// 		break;
// 	default:
// 		UE_LOGFMT(LogTemp, Error, "Unknown input mode: {InputMode}", static_cast<int32>(InputMode));
// 	}
// }
//
// void ALambdaSnailHUD::SetControllerOptions(FScreenPtr const& Screen, APlayerController* PlayerController) const
// {
// 	PlayerController->SetShowMouseCursor(Screen->bShowMouseCursor);
// 	SetInputMode(Screen->PreferredInputMode, PlayerController);
// }