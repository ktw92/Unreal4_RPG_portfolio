// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_PuzzleTrigger.h"
#include "PP_PlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APP_PuzzleTrigger::APP_PuzzleTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	My_Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	My_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	My_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SetRootComponent(MyBox);
}

// Called when the game starts or when spawned
void APP_PuzzleTrigger::BeginPlay()
{
	Super::BeginPlay();

	APP_PlayerController* temp = Cast< APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp)
	{
		Players = temp->GetMyParty();
	}
}

// Called every frame
void APP_PuzzleTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Players)
	{
		FVector dir = FVector::ZeroVector;
		float min = 123456789;
		for (auto& player : *Players)
		{
			float x = player->GetActorLocation().X;
			float y = player->GetActorLocation().Y;

			if (x + y < 300)
			{
				if (x + y < min)
				{
					min = x + y;
					dir = player->GetActorLocation() - GetActorLocation();
				}
			}
		}

		if (dir != FVector::ZeroVector)
		{
			dir.Normalize();
			SetActorRotation(FRotator(0.f, dir.Rotation().Yaw, 0.f));
		}
	}
}

void APP_PuzzleTrigger::RotationChangeOverlap()
{
	My_Mesh->AddRelativeRotation(FRotator(0, 90, 0));
	My_Mesh->ApplyRadiusDamage(1000.f, Super::GetActorLocation(), 50.f, 50.f, true);
}



