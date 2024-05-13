#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/HUD.h"
#include "LambdaSnailHUD.generated.h"

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
struct FLayerParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag LayerTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<ULambdaSnailUILayer> Layer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EInputMode InputMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bShowMouseCursor;
	
	/**
	 * If true the layer will never be hidden by another layer.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAlwaysVisible;
};

UCLASS(Blueprintable, BlueprintType)
class LAMBDASNAILMENUTOOLS_API ALambdaSnailHUD : public AHUD
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void RegisterLayer(FLayerParams const LayerParams);

	UFUNCTION(BlueprintCallable)
	void PushScreenToLayer(FGameplayTag const LayerTag, FGameplayTag const ScreenTag);
	
	UFUNCTION(BlueprintCallable)
	void PopScreenFromLayer(FGameplayTag const LayerTag);

	void CollapseAllExceptHUD();

private:

	UPROPERTY()
	TMap<FGameplayTag, FLayerParams> LayerMap;

	void SetInputMode(EInputMode InputMode, APlayerController* PlayerController) const;
};
