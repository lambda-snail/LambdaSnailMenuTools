#include "LambdaSnailHUD.h"

#include "Blueprint/UserWidget.h"
#include "MenuTags.h"

void ALambdaSnailHUD::PushWidget(FGameplayTag const WidgetTag, ESlateVisibility const Visibility)
{
	FWidgetMap const& Map = ResolveWidgetMap(WidgetTag);
	if(not Map.Contains(WidgetTag))
	{
		return;
	}

	FWidgetArray& ActiveWidgets = ResolveWidgetArray(WidgetTag);
	if(ActiveWidgets.Num() != 0)
	{
		if(ActiveWidgets.Last() == Map[WidgetTag])
		{
			// Active widget same as the one we're trying to push - do nothing
			return;	
		}

		TObjectPtr<UUserWidget> const CurrentWidget = ActiveWidgets.Last();
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	TObjectPtr<UUserWidget> const Widget = *Map.Find(WidgetTag);
	ActiveWidgets.Push(Widget);
	Widget->SetVisibility(Visibility);
}

void ALambdaSnailHUD::PopWidget(FGameplayTag const WidgetLayerTag)
{
	FWidgetArray& ActiveWidgets = ResolveWidgetArray(WidgetLayerTag);
	if(ActiveWidgets.Num() > 0)
	{
		TObjectPtr<UUserWidget> const CurrentWidget = ActiveWidgets.Pop(EAllowShrinking::No);
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		if(ActiveWidgets.Num() > 0)
		{
			TObjectPtr<UUserWidget> const NextWidget = ActiveWidgets.Last();
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
			MenuWidgets.Add(Tag, Widget);
		}
	}

	Super::BeginPlay();
}

ALambdaSnailHUD::FWidgetArray& ALambdaSnailHUD::ResolveWidgetArray(FGameplayTag WidgetTag)
{
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_InGameMenu))
	{
		return ActiveInGameMenuWidgets;
	}
	
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_Menu))
    {
    	return ActiveMenuWidgets;
    }
	
	return ActiveModalWidgets;
}

ALambdaSnailHUD::FWidgetMap& ALambdaSnailHUD::ResolveWidgetMap(FGameplayTag WidgetTag)
{
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_InGameMenu))
	{
		return InGameMenuWidgets;
	}
	
	if(WidgetTag.MatchesTag(TAG_LambdaSnail_Ui_Menu))
	{
		return MenuWidgets;
	}
	
	return ModalWidgets;
}