// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/HUD.h"
#include "Layers/LayersSubsystem.h"
#include "LambdaSnailHUD.generated.h"

class UUserWidget;

USTRUCT(BlueprintType)
struct FWidgetDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WidgetTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetType;
};

enum class ELayer : uint8
{
	HUD,
	InGameMenu,
	Menu,
	Modal
};

/**
 * A HUD class that manages different layers of a user interface. There are four layers:
 * - HUD, Always visible
 * - InGameMenu, Things like inventory screens etc.
 * - Menu, Pause menu or main menu
 * - Modals, Popups that should appear over all other menus
 *
 * The class automatically handles the visibility of lower layers when a widget is displayed
 * in a higher layer.
 */
UCLASS(Blueprintable, BlueprintType)
class LAMBDASNAILMENUTOOLS_API ALambdaSnailHUD : public AHUD
{
	GENERATED_BODY()
public:

	using FWidgetArray = TArray<TObjectPtr<UUserWidget>>;
	using FWidgetMap = TMap<FGameplayTag, TObjectPtr<UUserWidget>>;
	
	//void AddWidget(ELayer, FGameplayTag, UUserWidget*);
	//void AddWidget(ELayer, FGameplayTag, TSubclassOf<UUserWidget>&);

	// TODO: Do we need a stack for each layer? Or some layers?
	UFUNCTION(BlueprintCallable)
	void PushWidget(FGameplayTag const WidgetTag, ESlateVisibility const Visibility = ESlateVisibility::HitTestInvisible);

	UFUNCTION(BlueprintCallable)
	void PopWidget(FGameplayTag const WidgetLayerTag);


	//void CollapseAll(bool bShouldCollapseHud);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUserWidget> HUDWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> HudWidgetType;

	UPROPERTY(EditDefaultsOnly)
	TArray<FWidgetDefinition> InGameMenuWidgetDefinitions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FWidgetDefinition> MenuWidgetDefinitions;

	
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<UUserWidget>> InGameMenuWidgets {};
	TArray<TObjectPtr<UUserWidget>> ActiveInGameMenuWidgets {};
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<UUserWidget>> MenuWidgets {};
	TArray<TObjectPtr<UUserWidget>> ActiveMenuWidgets {};

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<UUserWidget>> ModalWidgets {};
	TArray<TObjectPtr<UUserWidget>> ActiveModalWidgets {};

private:
	FWidgetArray& ResolveWidgetArray(FGameplayTag WidgetTag);
	FWidgetMap& ResolveWidgetMap(FGameplayTag WidgetTag);
};
