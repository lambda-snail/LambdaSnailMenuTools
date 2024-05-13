#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/HUD.h"
#include "LambdaSnailHUD.generated.h"

class ULambdaSnailUILayer;
class UUserWidget;

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
	
	//void AddWidget(ELayer, FGameplayTag, UUserWidget*);
	//void AddWidget(ELayer, FGameplayTag, TSubclassOf<UUserWidget>&);
	
	UFUNCTION(BlueprintCallable)
	void RegisterLayer(FGameplayTag const LayerTag, ULambdaSnailUILayer* Layer);

	UFUNCTION(BlueprintCallable)
	void PushScreenToLayer(FGameplayTag const LayerTag, FGameplayTag const ScreenTag);
	
	UFUNCTION(BlueprintCallable)
	void PopScreenFromLayer(FGameplayTag const LayerTag);

	void CollapseAllExceptHUD();

private:

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<ULambdaSnailUILayer>> LayerMap;
};
