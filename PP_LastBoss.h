// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_LastBossAnim.h"
#include "PP_Monster.h"
#include "PP_LastBoss.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_LastBoss : public APP_Monster
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APP_LastBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ChangeAnimState(AnimType type);

	UPP_LastBossAnim* MyAnim;

	TSubclassOf<class APP_ProjectileBoom> NormalAttackEffect;

	TSubclassOf<class APP_DotDamageSkill> Skill1Effect;
	TSubclassOf<class APP_EffectBase> Skill1_HitEffect;

	TSubclassOf<class APP_ProjectileBoom> Skill2Effect;
	FVector Skill2Targetpos;

	TSubclassOf<class APP_ProjectileBoom> Skill3Effect;

	TSubclassOf<class APP_EffectBase> Skill4Effect;

	TSubclassOf<class APP_EffectBase> Skill5Effect;

	TSubclassOf<class APP_EffectBase> Skill6Effect;

	float SkillCoolTime[6];

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Attack1Fire();
	void Skill1Fire();
	void Skill2Fire(bool IsFirst = true);
	void Skill3Fire();
	void Skill4Fire();
	void Skill5Fire();
	void Skill6Fire();
};
