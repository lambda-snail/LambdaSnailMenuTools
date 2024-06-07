// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LambdaSnailActivatableWidget.h"
#include "LambdaSnailHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class LAMBDASNAILMENUTOOLS_API ULambdaSnailHudWidget : public ULambdaSnailActivatableWidget
{
	GENERATED_BODY()

public:
	void ShowInteractionPrompt(bool bIsVisible) const;

protected:
	UPROPERTY(meta = (BindWidget=InteractionPrompt, OptionalWidget=true))
	TObjectPtr<UCommonUserWidget> InteractionPrompt;
};
