#include "LambdaSnailHUD.h"

#include "LambdaSnailUILayer.h"
#include "Logging/StructuredLog.h"
#include "WorldPartition/DataLayer/DataLayerSubsystem.h"

void ALambdaSnailHUD::RegisterLayer(FLayerParams const LayerParams)
{
	LayerMap.Add(LayerParams.LayerTag, LayerParams);
	LayerParams.Layer->AddToViewport();
}

void ALambdaSnailHUD::PushScreenToLayer(FGameplayTag const LayerTag, FGameplayTag const ScreenTag)
{
	if(FLayerParams const* LayerParams = LayerMap.Find(LayerTag))
	{
		LayerParams->Layer->PushScreen(ScreenTag);
		
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if(PlayerController)
		{
			PlayerController->SetShowMouseCursor(LayerParams->bShowMouseCursor);
			SetInputMode(LayerParams->InputMode, PlayerController);
		}
	}
}

void ALambdaSnailHUD::PopScreenFromLayer(FGameplayTag const LayerTag)
{
	if(FLayerParams const* LayerParams = LayerMap.Find(LayerTag))
	{
		LayerParams->Layer->PopScreen();
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

void ALambdaSnailHUD::BeginPlay()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(not PlayerController)
	{
		return;
	}

	int32 priority {};
	for(FLayerCreationParams const& LayerCreationParam : LayerCreationParams)
	{
		ULambdaSnailUILayer* Layer = CreateWidget<ULambdaSnailUILayer>(PlayerController, LayerCreationParam.LayerClass);
		FLayerParams const LayerParams
		{
			.LayerTag = LayerCreationParam.LayerTag,
			.Layer = Layer,
			.InputMode = LayerCreationParam.InputMode,
			.bShowMouseCursor = LayerCreationParam.bShowMouseCursor,
			.bAlwaysVisible = LayerCreationParam.bAlwaysVisible,
			.Priority = priority++
		};
		
		RegisterLayer(LayerParams);
		LayerPriorities.Add(LayerParams.Priority, LayerParams.LayerTag);
	}
	
	Super::BeginPlay();
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
void ALambdaSnailHUD::SetInputMode(EInputMode InputMode, APlayerController* PlayerController) const
{
	switch(InputMode)
	{
	case EInputMode::GameOnly:
		{
			FInputModeGameOnly InputModeData{};
			PlayerController->SetInputMode(InputModeData);
		}
		break;
	case EInputMode::UIOnly:
		{
			FInputModeUIOnly InputModeData{};
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
		}
		break;
	case EInputMode::GameAndUI:
		{
			FInputModeGameAndUI InputModeData{};
			PlayerController->SetInputMode(InputModeData);
		}
		break;
	default:
		UE_LOGFMT(LogTemp, Error, "Unknown input mode: {InputMode}", static_cast<int32>(InputMode));
	}
}
//
// void ALambdaSnailHUD::SetControllerOptions(FScreenPtr const& Screen, APlayerController* PlayerController) const
// {
// 	PlayerController->SetShowMouseCursor(Screen->bShowMouseCursor);
// 	SetInputMode(Screen->PreferredInputMode, PlayerController);
// }