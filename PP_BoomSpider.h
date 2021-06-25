// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_BoomspiderAnim.h"
#include "PP_Monster.h"
#include "PP_BoomSpider.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_BoomSpider : public APP_Monster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APP_BoomSpider();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPP_BoomspiderAnim* MyAnim;


	virtual void ChangeAnimState(AnimType type);

	TSubclassOf<class APP_EffectBase> NormalHitEffect;
	TSubclassOf<class APP_EffectBase> BoomEffect;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NormalAttack();
	void DeathBoom();
};
