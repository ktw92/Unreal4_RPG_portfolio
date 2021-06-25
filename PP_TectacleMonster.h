// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_Monster.h"
#include "PP_TentacleMonsterAnim.h"
#include "PP_TectacleMonster.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_TectacleMonster : public APP_Monster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APP_TectacleMonster();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPP_TentacleMonsterAnim* MyAnim;


	virtual void ChangeAnimState(AnimType type);

	TSubclassOf<class APP_EffectBase> NormalHitEffect;
	TSubclassOf<class APP_EffectBase> BoomEffect;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NormalAttack();
	virtual void NormalAttack2();
	void DeathBoom();
};
