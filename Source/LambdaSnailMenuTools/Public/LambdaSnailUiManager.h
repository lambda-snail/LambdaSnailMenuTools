#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "NativeGameplayTags.h"

#include "LambdaSnailUiManager.generated.h"

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

UCLASS(Abstract, Blueprintable, BlueprintType)
class LAMBDASNAILMENUTOOLS_API ULambdaSnailUiManager : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterLayer(FGameplayTag const Tag, ULambdaSnailUILayer* Layer);

	UFUNCTION(BlueprintCallable)
	void PushWidgetToLayer(FGameplayTag const LayerTag, TSubclassOf<ULambdaSnailActivatableWidget> WidgetClass);
	
	UFUNCTION(BlueprintCallable)
	void PopWidgetFromLayer(FGameplayTag const LayerTag);
	
private:

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<ULambdaSnailUILayer>> LayerMap;
};
