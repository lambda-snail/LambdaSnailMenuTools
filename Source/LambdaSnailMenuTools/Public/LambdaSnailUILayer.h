#pragma once

#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"

#include "LambdaSnailUILayer.generated.h"

class UNamedSlot;
class ULambdaSnailScreen;

USTRUCT()
struct FScreenCreationParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULambdaSnailScreen> ScreenType;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Tag;
};

UCLASS(Blueprintable, BlueprintType)
class ULambdaSnailUILayer : public UUserWidget
{
	GENERATED_BODY()

	using FScreenPtr = TStrongObjectPtr<ULambdaSnailScreen>;
	using FScreenArray = TArray<FScreenPtr>;
	using FScreenMap = TMap<FGameplayTag, FScreenPtr>;
	
public:
	virtual bool Initialize() override;
	
	void PushScreen(FGameplayTag const& Tag);
	void PushTemporaryScreen(TSubclassOf<ULambdaSnailScreen> const& Widget) const;
	void PopScreen();
	
	void RegisterCachedScreen(FGameplayTag const& Tag, UUserWidget* Widget) const;

	void HideTopWidget() const;
	void ShowTopWidget() const;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNamedSlot> LayerSlot;
	
	/**
	 * These screens will be created and added to the cache when the layer is initialized.
	 * Equivalent to calling PushScreen for each widget.
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FScreenCreationParams> InitialScreens;
	
private:

	FScreenArray ActiveScreenStack {};
	FScreenMap CachedScreens {};
};
