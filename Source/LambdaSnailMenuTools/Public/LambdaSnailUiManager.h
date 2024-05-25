#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "NativeGameplayTags.h"

#include "LambdaSnailUiManager.generated.h"

class UCommonActivatableWidget;
class UOverlay;
class ULambdaSnailActivatableWidget;
class ULambdaSnailUILayer;
class UUserWidget;

UENUM()
enum class EInputMode : uint8
{
	GameOnly,
	UIOnly,
	GameAndUI
};

USTRUCT(Blueprintable, BlueprintType)
struct FLayerRegistrationParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite)
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite)
	ULambdaSnailUILayer* Layer;
	
	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite)
	bool bIsBackgroundLayer = false;
};

USTRUCT()
struct FLayerContainer
{
	GENERATED_BODY()
	
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
	void PushWidgetToLayer(FGameplayTag const LayerTag, TSubclassOf<ULambdaSnailActivatableWidget> WidgetClass);
	
	UFUNCTION(BlueprintCallable)
	void PopWidgetFromLayer(FGameplayTag const LayerTag);

protected:
	virtual void NativeDestruct() override;
	
private:

	UPROPERTY()
	TMap<FGameplayTag, FLayerContainer> LayerMap;

	void WidgetContainer_OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget);
};
