// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_PuzzleSwitchWall.h"

// Sets default values
APP_PuzzleSwitchWall::APP_PuzzleSwitchWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APP_PuzzleSwitchWall::BeginPlay()
{
	Super::BeginPlay();
	OriginPos = GetActorLocation();
	Tags.Add(FName(TagName));
	isMoving = true;
}

// Called every frame
void APP_PuzzleSwitchWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isMoving)
	{
		float deltapos = 500 * DeltaTime;
		if (IsFirst)
		{
			float temz = GetActorLocation().Z + deltapos;
			if (temz < OriginPos.Z + 500)
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, temz));
			}
			else
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, OriginPos.Z + 500));
				isMoving = false;
			}
		}
		else
		{
			float temz = GetActorLocation().Z - deltapos;
			if (temz > OriginPos.Z - 500)
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, temz));
			}
			else
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, OriginPos.Z - 500));
				isMoving = false;
			}
		}
	}
}

void APP_PuzzleSwitchWall::ChangePos()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Change Hit")));

	if (!isMoving)
	{
		IsFirst = !IsFirst;
		isMoving = true;
	}
}

