// Fill out your copyright notice in the Description page of Project Settings.

#include "GameWidget.h"

void UGameWidget::Load() 
{
	const FName TextBlockName("ScoretextBlock");
	if (!ScoreText) {
		ScoreText = (UTextBlock*)(WidgetTree->FindWidget(TextBlockName));
	}
}

void UGameWidget::SetScore(int Score) 
{
	if (ScoreText) {
		ScoreText->SetText(FText::FromString("Score: " + FString::FromInt(Score)));
	}
}

void UGameWidget::OnGameOver(int Score)
{
	if (ScoreText) {
		ScoreText->SetText(FText::FromString("GAME OVER!\nScore: " + FString::FromInt(Score) + "\nPress R to restart"));
	}
}
