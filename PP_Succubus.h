// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_SuccubusAnim.h"
#include "PP_Monster.h"
#include "PP_Succubus.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_Succubus : public APP_Monster
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APP_Succubus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPP_SuccubusAnim* MyAnim;

	virtual void ChangeAnimState(AnimType type);

	TSubclassOf<class APP_EffectBase> MySkill1Effect;
	TSubclassOf<class APP_EffectBase> MySkill2Effect;
	TSubclassOf<class APP_EffectBase> Skill3Effect;
	TSubclassOf<class APP_EffectBase> Normal2HitEffect;
	TSubclassOf<class APP_ArachBossSkill1> Skill1Effect;
	TSubclassOf<class APP_DotDamageSkill> Skill2Effect;

	TSubclassOf<class APP_ProjectileAttack> NormalAttackProj;
	TSubclassOf<class APP_ProjectileAttack> Skill5Proj;

	float Skill1CoolAcc;
	float Skill2CoolAcc;
	float Skill3CoolAcc;

	float SkillCoolTime[5];

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Attack1Fire();
	void Skill1Fire();
	void Skill2Fire(bool right = true);
	void Skill3Fire();
	void Skill4Fire();
	void Skill5Fire();
	void DeathBoom();
	virtual void Idle()
	{
		Super::Idle();
		isSkillCast = false;
	}
};
