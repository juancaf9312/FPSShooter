// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSShooterGameMode.h"
#include "NPC/EnemyController.h"
#include "GameWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

void AFPSShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);

	((UGameWidget*)CurrentWidget)->Load();

	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("Restart", IE_Pressed, this, &AFPSShooterGameMode::OnRestart).bExecuteWhenPaused = true;

}

void AFPSShooterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimer += DeltaTime;
	EnemyTimer -= DeltaTime;

	if (EnemyTimer <= 0.f) {
		float DifficultyPercentage = FMath::Min(GameTimer / TIME_TO_MINIMUM_INTERVAL, 1.f);
		EnemyTimer = MAXIMUM_INTERVAL - (MAXIMUM_INTERVAL - MINIMUM_INTERVAL) * DifficultyPercentage;

		if (GetWorld()) {
			float Distance = 800.f;
			float RandomAngle = FMath::RandRange(0.f, 360.f);

			FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			FVector EnemyLocation = PlayerLocation;

			EnemyLocation.X += FMath::Cos(FMath::DegreesToRadians(RandomAngle))*Distance;
			EnemyLocation.Y += FMath::Sin(FMath::DegreesToRadians(RandomAngle))*Distance;
			EnemyLocation.Z = 202.f;

			AEnemyController* Enemy = GetWorld()->SpawnActor<AEnemyController>(EnemyBlueprint, EnemyLocation, FRotator::ZeroRotator);

			Enemy->Direction = (PlayerLocation - EnemyLocation).GetSafeNormal();
		}
	}

}

void AFPSShooterGameMode::IncrementScore()
{
	Score += 100;
	((UGameWidget*)CurrentWidget)->SetScore(Score);
}

void AFPSShooterGameMode::OnGameOver()
{
	((UGameWidget*)CurrentWidget)->OnGameOver(Score);
}

void AFPSShooterGameMode::OnRestart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AFPSShooterGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);

		if (CurrentWidget)CurrentWidget->AddToViewport();
	}
}
