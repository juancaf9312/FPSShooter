// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "FPSShooterGameMode.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/ShapeComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AEnemyController::AEnemyController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootBox->bGenerateOverlapEvents = true;
	RootBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyController::OnOverlap);

	SetRootComponent(RootBox);

}

// Called when the game starts or when spawned
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.X += Direction.X * Speed * DeltaTime;
	NewLocation.Y += Direction.Y * Speed * DeltaTime;
	SetActorLocation(NewLocation);

}

void AEnemyController::OnOverlap(UPrimitiveComponent * OverlapComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult) 
{
	if (OtherActor == (AActor*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
		((AFPSShooterGameMode*)GetWorld()->GetAuthGameMode())->OnGameOver();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
	else if (OtherActor->GetName().Contains("Projectile")) {
		((AFPSShooterGameMode*)GetWorld()->GetAuthGameMode())->IncrementScore();
		OtherActor->Destroy();
		this->Destroy();
	}
}
