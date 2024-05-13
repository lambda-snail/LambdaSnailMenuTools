#pragma once

#include "Blueprint/UserWidget.h"

#include "LambdaSnailScreen.generated.h"

UCLASS()
class ULambdaSnailScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnVisibilityChange(bool bIsVisible);
	
};