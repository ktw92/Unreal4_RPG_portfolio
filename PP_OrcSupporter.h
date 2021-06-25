// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_Monster.h"
#include "PP_OrcSupporterAnim.h"
#include "PP_OrcSupporter.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_OrcSupporter : public APP_Monster
{
	GENERATED_BODY()
public:
	APP_OrcSupporter();
protected:
	virtual void BeginPlay() override;
	class UPP_OrcSupporterAnim* MyAnim;
	virtual void ChangeAnimState(AnimType type);
	TSubclassOf<class APP_ProjectileAttack> NormalAttackProj;
	TSubclassOf<class APP_EffectBase> Skill1Effect;
	UPROPERTY()
	float SkillCoolTime;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Idle();
	virtual void NormalAttack();
	virtual void Death();
	virtual void Paralysis();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	void NormalAttackFire();
	void Skill1Fire();
	virtual void Skill1();
};
