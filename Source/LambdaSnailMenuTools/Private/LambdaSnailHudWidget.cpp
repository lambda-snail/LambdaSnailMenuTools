// Fill out your copyright notice in the Description page of Project Settings.


#include "LambdaSnailHudWidget.h"

#include "LambdaSnailUiManager.h"

void ULambdaSnailHudWidget::ShowInteractionPrompt(bool bIsVisible) const
{
	if(InteractionPrompt)
	{
		InteractionPrompt->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
