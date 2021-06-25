// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_GoblinAnim.h"
#include "PP_Monster.h"
#include "PP_Weapon.h"
#include "KTWFunc.h"
#include "PP_EffectBase.h"
#include "PP_Goblin.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_Goblin : public APP_Monster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APP_Goblin();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DetectRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPP_GoblinAnim* MyAnim;
	APP_Weapon* GoblinWeapon;

	virtual void ChangeAnimState(AnimType type);

	TSubclassOf<class APP_EffectBase> NormalHitEffect;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Idle();
	virtual void NormalAttack();
	virtual void Death();
	virtual void Paralysis();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
};
