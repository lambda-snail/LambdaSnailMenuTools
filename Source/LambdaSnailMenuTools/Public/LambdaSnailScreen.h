#pragma once

#include "Blueprint/UserWidget.h"

#include "LambdaSnailScreen.generated.h"

DECLARE_DELEGATE(FOnRequestCloseSelf)

UCLASS()
class ULambdaSnailScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnVisibilityChange(bool bIsVisible);
	
	FOnRequestCloseSelf OnRequestCloseSelf;

protected:
	UFUNCTION(BlueprintCallable)
	void RequestCloseSelf() const;
};
