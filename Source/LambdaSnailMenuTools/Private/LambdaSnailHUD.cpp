#include "LambdaSnailHUD.h"

#include "Blueprint/UserWidget.h"
#include "MenuTags.h"

void ALambdaSnailHUD::PushWidget(FGameplayTag const WidgetTag, ESlateVisibility const Visibility)
{
	if(not WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_InGameMenu.GetTag()) or not InGameMenuWidgets.Contains(WidgetTag))
	{
		return;
	}

	if(ActiveInGameMenuWidgets.Num() > 0)
	{
		TObjectPtr<UUserWidget> const CurrentWidget = ActiveInGameMenuWidgets.Last();
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	TObjectPtr<UUserWidget> const Widget = *InGameMenuWidgets.Find(WidgetTag);
	ActiveInGameMenuWidgets.Push(Widget);
	Widget->SetVisibility(Visibility);
}

void ALambdaSnailHUD::PopWidget(FGameplayTag const WidgetLayerTag)
{
	if(not WidgetLayerTag.MatchesTag(TAG_LambdaSnail_Ui_InGameMenu.GetTag()))
	{
		return;
	}

	if(ActiveInGameMenuWidgets.Num() > 0)
	{
		TObjectPtr<UUserWidget> const CurrentWidget = ActiveInGameMenuWidgets.Pop(EAllowShrinking::No);
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		if(ActiveInGameMenuWidgets.Num() > 0)
		{
			TObjectPtr<UUserWidget> const NextWidget = ActiveInGameMenuWidgets.Last();
			NextWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void ALambdaSnailHUD::BeginPlay()
{
	if(APlayerController* Controller = GetOwningPlayerController())
	{
		HUDWidget = CreateWidget<UUserWidget>(Controller, HudWidgetType);
		HUDWidget->AddToViewport();
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		FInputModeGameOnly const InputMode;
		Controller->SetInputMode(InputMode);

		InGameMenuWidgets = TMap<FGameplayTag, TObjectPtr<UUserWidget>>();
		for(auto& [Tag, Type] : InGameMenuWidgetDefinitions)
		{
			UUserWidget* Widget = CreateWidget<UUserWidget>(Controller, Type);
			Widget->AddToViewport();
			Widget->SetVisibility(ESlateVisibility::Collapsed);
			InGameMenuWidgets.Add(Tag, Widget);
		}

		for(auto& [Tag, Type] : MenuWidgetDefinitions)
		{
			UUserWidget* Widget = CreateWidget<UUserWidget>(Controller, Type);
			Widget->AddToViewport();
			Widget->SetVisibility(ESlateVisibility::Collapsed);
			InGameMenuWidgets.Add(Tag, Widget);
		}
	}

	Super::BeginPlay();
}
