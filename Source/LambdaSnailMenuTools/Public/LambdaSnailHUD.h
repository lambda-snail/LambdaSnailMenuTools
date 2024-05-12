#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/HUD.h"
#include "LambdaSnailHUD.generated.h"

class UUserWidget;

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	UIOnly,
	GameOnly,
	UIAndGame
};

USTRUCT(BlueprintType)
struct FScreenDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WidgetTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetType;

	UPROPERTY(EditDefaultsOnly)
	ESlateVisibility PreferredVisibility { ESlateVisibility::HitTestInvisible };
	
	UPROPERTY(EditDefaultsOnly)
	EInputMode PreferredInputMode;

	UPROPERTY(EditDefaultsOnly)
	bool bShowMouseCursor { false };
};

USTRUCT()
struct FManagedScreen
{
	GENERATED_BODY()

	FGameplayTag WidgetTag;

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget;
	bool bShowMouseCursor { false };
	
	EInputMode PreferredInputMode;

	ESlateVisibility PreferredVisibility;
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

	using FScreenPtr = TSharedPtr<FManagedScreen>;
	using FScreenArray = TArray<FScreenPtr>;
	using FScreenMap = TMap<FGameplayTag, FScreenPtr>;
	using FWidgetDefinitionArray = TArray<FScreenDefinition>;
	
	//void AddWidget(ELayer, FGameplayTag, UUserWidget*);
	//void AddWidget(ELayer, FGameplayTag, TSubclassOf<UUserWidget>&);
	
	UFUNCTION(BlueprintCallable)
	void PushScreen(FGameplayTag const WidgetTag);

	UFUNCTION(BlueprintCallable)
	void PopScreen(FGameplayTag const WidgetLayerTag);
	void InitDataStructures(APlayerController* Controller, FScreenMap& OutMap, FWidgetDefinitionArray const& InDefinitionsArray);
	
	//void CollapseAll(bool bShouldCollapseHud);

protected:
	virtual void BeginPlay() override;
	
	FScreenPtr HUDScreen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScreenDefinition HudScreenDefinition;

	UPROPERTY(EditDefaultsOnly)
	TArray<FScreenDefinition> InGameMenuScreenDefinitions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FScreenDefinition> MenuScreenDefinitions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FScreenDefinition> ModalScreenDefinitions;
	
	FScreenMap		InGameMenuScreens {};
	FScreenArray	ActiveInGameMenuScreens {};
	
	FScreenMap		MenuScreens {};
	FScreenArray	ActiveMenuScreens {};
	
	FScreenMap		ModalScreens {};
	FScreenArray	ActiveModalScreens {};

private:
	FScreenArray& ResolveScreenArray(FGameplayTag WidgetTag);
	FScreenMap& ResolveScreenMap(FGameplayTag WidgetTag);
	
	void SetControllerOptions(FScreenPtr const& Screen) const;
	void SetControllerOptions(FScreenPtr const& Screen, APlayerController* PlayerController) const;
	void SetInputMode(EInputMode InputMode, APlayerController* PlayerController) const;
	void CreateScreenPtr(APlayerController* Controller, const FScreenDefinition& WidgetDefinition, ALambdaSnailHUD::FScreenPtr& OutScreen) const;
};
