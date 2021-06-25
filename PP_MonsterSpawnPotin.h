// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_Monster.h"
#include "GameFramework/Actor.h"
#include "PP_MonsterSpawnPotin.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_MonsterSpawnPotin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_MonsterSpawnPotin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APP_Monster>	MonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	RespawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float	PatrolTime;

	APP_Monster* SpawnedMonster;
	float	SpawnTimeAcc;
	float	PatrolTImeAcc;

	void OrderMonsterPatrol();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxRespawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Restart() { SpawnedMonster = nullptr; SpawnTimeAcc = 0.f; }
	void SetRespawn(float time_interval, int count)
	{
		MaxRespawn = count;
		RespawnTime = time_interval;
	}
};
