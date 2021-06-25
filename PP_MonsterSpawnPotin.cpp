// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_MonsterSpawnPotin.h"


// Sets default values
APP_MonsterSpawnPotin::APP_MonsterSpawnPotin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("SpawnPoint");
}

// Called when the game starts or when spawned
void APP_MonsterSpawnPotin::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnTimeAcc = 0.f;
	SpawnedMonster = nullptr;
	//if (MaxRespawn == 0)
	//	MaxRespawn = 1;
}

// Called every frame
void APP_MonsterSpawnPotin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SpawnedMonster && MaxRespawn)
	{ 
		SpawnTimeAcc += DeltaTime;
		if (RespawnTime <= SpawnTimeAcc)
		{
			MaxRespawn--;
			SpawnTimeAcc = 0.f;
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SpawnedMonster = GetWorld()->SpawnActor<APP_Monster>(MonsterType, GetActorLocation(),
				GetActorRotation(), param);
			SpawnedMonster->SetSpawnPoint(this);
		}
	}
}

void APP_MonsterSpawnPotin::OrderMonsterPatrol()
{
	if (SpawnedMonster)
	{
		;
		//float x = GetActorLocation().X + 
	}
}

