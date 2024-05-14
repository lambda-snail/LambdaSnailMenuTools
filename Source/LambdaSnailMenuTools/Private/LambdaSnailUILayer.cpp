#include "LambdaSnailScreen.h"
#include "LambdaSnailUILayer.h"

#include "Components/NamedSlot.h"

bool ULambdaSnailUILayer::Initialize()
{
	UWorld const* World = GetWorld();
	check(World);

	if(APlayerController* Controller = World->GetFirstPlayerController())
	{
		for(const auto& [ScreenType, Tag] : InitialScreens)
		{
			ULambdaSnailScreen* Screen = CreateWidget<ULambdaSnailScreen>(Controller, ScreenType);
			CachedScreens.Add(Tag, FScreenPtr(Screen));
		}
	}
	
	return Super::Initialize();
}

void ULambdaSnailUILayer::PushScreen(FGameplayTag const& Tag)
{
	FScreenPtr const* Screen = CachedScreens.Find(Tag);
	if(not Screen)
	{
		return;
	}

	if(ActiveScreenStack.Num() != 0)
	{
		if(ActiveScreenStack.Last() == *Screen)
		{
			// Active widget same as the one we're trying to push - do nothing
			return;	
		}

		// Hide currently active screen
		HideTopWidget();
	}

	ActiveScreenStack.Push(*Screen);
	ShowTopWidget();
}

void ULambdaSnailUILayer::PushTemporaryScreen(TSubclassOf<ULambdaSnailScreen> const& Widget) const
{
}

void ULambdaSnailUILayer::PopScreen() 
{
	if(ActiveScreenStack.Num() == 0)
	{
		return;
	}

	HideTopWidget();
	FScreenPtr const PreviousScreen = ActiveScreenStack.Pop(EAllowShrinking::No);

	//PreviousScreen->OnVisibilityChange(false);
	
	ShowTopWidget();
}

void ULambdaSnailUILayer::RegisterCachedScreen(FGameplayTag const& Tag, UUserWidget* Widget) const
{
}

void ULambdaSnailUILayer::HideTopWidget() const
{
	if(ActiveScreenStack.Num() > 0)
	{
		FScreenPtr const TopWidget = ActiveScreenStack.Last();
		TopWidget->SetVisibility(ESlateVisibility::Collapsed); // TODO: Make more dynamic, per-layer basis?
		LayerSlot->ClearChildren();

		TopWidget->OnRequestCloseSelf.Unbind();
	}
}

void ULambdaSnailUILayer::ShowTopWidget()
{
	if(ActiveScreenStack.Num() > 0)
	{
		FScreenPtr const NextWidget = ActiveScreenStack.Last();
		NextWidget->SetVisibility(ESlateVisibility::Visible);
		LayerSlot->AddChild(NextWidget.Get());

		NextWidget->OnRequestCloseSelf.BindUObject(this, &ThisClass::Screen_OnRequestCloseSelf);
	}
}

bool ULambdaSnailUILayer::IsEmpty() const
{
	return ActiveScreenStack.Num() == 0;
}

void ULambdaSnailUILayer::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULambdaSnailUILayer::NativeDestruct()
{
	ActiveScreenStack.Empty();
	CachedScreens.Empty();
	
	Super::NativeDestruct();
}

void ULambdaSnailUILayer::Screen_OnRequestCloseSelf()
{
	PopScreen();

	if(ActiveScreenStack.Num() == 0 and OnLayerTopScreenAutoClose.IsBound())
	{
		OnLayerTopScreenAutoClose.Execute();
	}
}
