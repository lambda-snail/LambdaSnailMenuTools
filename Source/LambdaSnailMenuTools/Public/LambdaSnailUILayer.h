#pragma once

#include "LambdaSnailUiManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "LambdaSnailUILayer.generated.h"

class UNamedSlot;
class ULambdaSnailActivatableWidget;

USTRUCT(Blueprintable)
struct FDefaultLayerWidgetInitializationParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULambdaSnailActivatableWidget> WidgetClass;
};

UCLASS(Blueprintable, BlueprintType)
class ULambdaSnailUILayer : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Initialize();
	
protected:

	// The widgets that will exist on the layer from the start
	UPROPERTY(EditAnywhere)
	TArray<FDefaultLayerWidgetInitializationParams> DefaultWidgets;
};
