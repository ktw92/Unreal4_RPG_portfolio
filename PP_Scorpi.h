// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_ScorpiAnim.h"
#include "PP_Monster.h"
#include "PP_Scorpi.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_Scorpi : public APP_Monster
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APP_Scorpi();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPP_ScorpiAnim* MyAnim;

	virtual void ChangeAnimState(AnimType type);

	TSubclassOf<class APP_EffectBase> NormalHitEffect;

	UPROPERTY()
	FVector goal;

	UPROPERTY()
	float PatrolTime;
	UPROPERTY()
	float PatrolAcc;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NormalAttack();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

};
