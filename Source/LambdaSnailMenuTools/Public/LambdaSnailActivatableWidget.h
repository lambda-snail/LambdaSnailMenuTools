#pragma once

#include "CommonActivatableWidget.h"
#include "LambdaSnailActivatableWidget.generated.h"

UCLASS()
class ULambdaSnailActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	ULambdaSnailActivatableWidget();

	/**
	 * Returns a pointer to the owning player controller. Convenience function for blueprint subclasses. 
	 */
	UFUNCTION(BlueprintCallable)
	APlayerController* GetOwningPlayerController() const;

	/**
	 * Returns a pointer to the local player. Convenience function for blueprint subclasses. 
	 */
	UFUNCTION(BlueprintCallable)
	ULocalPlayer* GetLocalPlayer() const;
};
