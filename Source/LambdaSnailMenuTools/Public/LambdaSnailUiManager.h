#pragma once

#include "CoreMinimal.h"
#include "CommonInputModeTypes.h"
#include "CommonUserWidget.h"
#include "LambdaSnailUILayer.h"
#include "NativeGameplayTags.h"

#include "LambdaSnailUiManager.generated.h"

class UCommonActivatableWidgetStack;
class UCommonActivatableWidget;
class UOverlay;
class ULambdaSnailActivatableWidget;
class UUserWidget;

USTRUCT(BlueprintType)
struct FLayerRegistrationParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ULambdaSnailUILayer> Layer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBackgroundLayer = false;
};

USTRUCT()
struct FLayerContainer
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ULambdaSnailUILayer> Layer;
	bool bIsBackgroundLayer = false;
	FDelegateHandle OnWidgetChangeDelegateHandle;
};

UCLASS(Abstract, Blueprintable, BlueprintType)
class LAMBDASNAILMENUTOOLS_API ULambdaSnailUiManager : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterLayer(FLayerRegistrationParams LayerRegistrationParams);

	UFUNCTION(BlueprintCallable)
	void PushWidgetToLayer(FGameplayTag const& LayerTag, TSubclassOf<ULambdaSnailActivatableWidget> WidgetClass);

	/**
	 * Push the given widget to the stack, and run the provided initialization function before displaying the widget
	 * in the viewport. 
	 */
	template<typename TWidget = ULambdaSnailActivatableWidget>
	void PushWidgetToLayer(FGameplayTag const LayerTag, TSubclassOf<TWidget> WidgetClass, TFunctionRef<void(TWidget&)> WidgetInitializer);
	
	UFUNCTION(BlueprintCallable)
	void PopWidgetFromLayer(FGameplayTag const LayerTag);
	
protected:
	virtual void NativeDestruct() override;

	// The input configuration to revert to when all non-background widgets are deactivated
	UPROPERTY(EditDefaultsOnly, Category = "LambdaSnail|UI Manager")
	FUIInputConfig DefaultInputConfig = { ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently };
	
private:

	UPROPERTY()
	TMap<FGameplayTag, FLayerContainer> LayerMap;

	void WidgetContainer_OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget);
};

template <typename TWidget>
void ULambdaSnailUiManager::PushWidgetToLayer(FGameplayTag const LayerTag, TSubclassOf<TWidget> WidgetClass, TFunctionRef<void(TWidget&)> WidgetInitializer)
{
	static_assert(std::is_base_of_v<ULambdaSnailActivatableWidget, TWidget>);
	if(FLayerContainer const* Layer = LayerMap.Find(LayerTag))
	{
		Layer->Layer->AddWidget(WidgetClass, WidgetInitializer);
	}
}
