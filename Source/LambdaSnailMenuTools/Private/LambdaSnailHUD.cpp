#include "LambdaSnailHUD.h"

#include "Blueprint/UserWidget.h"
#include "MenuTags.h"
#include "Logging/StructuredLog.h"

void ALambdaSnailHUD::PushScreen(FGameplayTag const WidgetTag)
{
	FScreenMap const& Map = ResolveScreenMap(WidgetTag);
	if(not Map.Contains(WidgetTag))
	{
		return;
	}

	FScreenArray& ActiveWidgets = ResolveScreenArray(WidgetTag);
	if(ActiveWidgets.Num() != 0)
	{
		if(ActiveWidgets.Last() == Map[WidgetTag])
		{
			// Active widget same as the one we're trying to push - do nothing
			return;	
		}

		FScreenPtr const CurrentScren = ActiveWidgets.Last();
		CurrentScren->Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	FScreenPtr const Screen = *Map.Find(WidgetTag);
	ActiveWidgets.Push(Screen);
	Screen->Widget->SetVisibility(Screen->PreferredVisibility);

	SetControllerOptions(Screen);
}

void ALambdaSnailHUD::PopScreen(FGameplayTag const WidgetLayerTag)
{
	FScreenArray& ActiveWidgets = ResolveScreenArray(WidgetLayerTag);
	if(ActiveWidgets.Num() > 0)
	{
		FScreenPtr const CurrentWidget = ActiveWidgets.Pop(EAllowShrinking::No);
		CurrentWidget->Widget->SetVisibility(ESlateVisibility::Collapsed);
		
		if(ActiveWidgets.Num() > 0)
		{
			FScreenPtr const NextWidget = ActiveWidgets.Last();
			//NextWidget->Widget->SetVisibility(NextWidget->PreferredVisibility);
			SetControllerOptions(NextWidget);
		}
		else
		{
			SetControllerOptions(HUDScreen);
		}
	}
}

void ALambdaSnailHUD::CreateScreenPtr(APlayerController* Controller, const FScreenDefinition& WidgetDefinition, FScreenPtr& OutScreen) const
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(Controller, WidgetDefinition.WidgetType);
	Widget->AddToViewport();
	Widget->SetVisibility(ESlateVisibility::Collapsed);
	
	OutScreen = MakeShared<FManagedScreen>();
	OutScreen->WidgetTag = WidgetDefinition.WidgetTag;
	OutScreen->Widget = Widget;
	OutScreen->bShowMouseCursor = WidgetDefinition.bShowMouseCursor;
	OutScreen->PreferredInputMode = WidgetDefinition.PreferredInputMode;
	OutScreen->PreferredVisibility =  WidgetDefinition.PreferredVisibility;
}

void ALambdaSnailHUD::TearDown()
{
	CollapseAllExceptHUD();

	static auto CleanUp = [](FScreenPtr const& ScreenPointer)
	{
		ScreenPointer->Widget->RemoveFromParent();
	};
	
	for ( auto const& [Tag, Screen] : InGameMenuScreens)
	{
		CleanUp(Screen);
	}

	for ( auto const& [Tag, Screen] : MenuScreens)
	{
		CleanUp(Screen);
	}

	for ( auto const& [Tag, Screen] : ModalScreens)
	{
		CleanUp(Screen);
	}
}

void ALambdaSnailHUD::InitDataStructures(APlayerController* Controller, FScreenMap& OutMap, FWidgetDefinitionArray const& InDefinitionsArray)
{
	for(auto& WidgetDefinition : InDefinitionsArray)
	{
		FScreenPtr Screen;
		CreateScreenPtr(Controller, WidgetDefinition, Screen);

		OutMap.Add(WidgetDefinition.WidgetTag, Screen);
	}
}

void ALambdaSnailHUD::CollapseAllExceptHUD()
{
	for ( auto const& [Tag, Screen] : InGameMenuScreens)
	{
		Screen->Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	for ( auto const& [Tag, Screen] : MenuScreens)
	{
		Screen->Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	for ( auto const& [Tag, Screen] : ModalScreens)
	{
		Screen->Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	ActiveInGameMenuScreens.Empty();
	ActiveMenuScreens.Empty();
	ActiveModalScreens.Empty();

	if(HUDScreen)
	{
		SetControllerOptions(HUDScreen);
		HUDScreen->Widget->SetVisibility(HUDScreen->PreferredVisibility);	
	}
}

void ALambdaSnailHUD::BeginPlay()
{
	if(APlayerController* Controller = GetOwningPlayerController())
	{
		if(HudScreenDefinition.WidgetType)
		{
			CreateScreenPtr(Controller, HudScreenDefinition, HUDScreen);
			HUDScreen->Widget->SetVisibility(HUDScreen->PreferredVisibility);
		}

		InitDataStructures(Controller, InGameMenuScreens, InGameMenuScreenDefinitions);
		InitDataStructures(Controller, MenuScreens, MenuScreenDefinitions);
		InitDataStructures(Controller, ModalScreens, ModalScreenDefinitions);
	}

	Super::BeginPlay();
}

void ALambdaSnailHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TearDown();
	Super::EndPlay(EndPlayReason);
}

ALambdaSnailHUD::FScreenArray& ALambdaSnailHUD::ResolveScreenArray(FGameplayTag WidgetTag)
{
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_InGameMenu))
	{
		return ActiveInGameMenuScreens;
	}
	
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_Menu))
    {
    	return ActiveMenuScreens;
    }
	
	return ActiveModalScreens;
}

ALambdaSnailHUD::FScreenMap& ALambdaSnailHUD::ResolveScreenMap(FGameplayTag WidgetTag)
{
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_InGameMenu))
	{
		return InGameMenuScreens;
	}
	
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_Menu))
	{
		return MenuScreens;
	}
	
	return ModalScreens;
}

void ALambdaSnailHUD::SetControllerOptions(FScreenPtr const& Screen) const
{
	UWorld const* World = GetWorld();
	if(not World)
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(not PlayerController)
	{
		return;
	}

	SetControllerOptions(Screen, PlayerController);
}

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
	case EInputMode::UIAndGame:
		{
			FInputModeGameAndUI InputModeData{};
			PlayerController->SetInputMode(InputModeData);
		}
		break;
	default:
		UE_LOGFMT(LogTemp, Error, "Unknown input mode: {InputMode}", static_cast<int32>(InputMode));
	}
}

void ALambdaSnailHUD::SetControllerOptions(FScreenPtr const& Screen, APlayerController* PlayerController) const
{
	PlayerController->SetShowMouseCursor(Screen->bShowMouseCursor);
	SetInputMode(Screen->PreferredInputMode, PlayerController);
}