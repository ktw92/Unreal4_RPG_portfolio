// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_Monster.h"
#include "PP_ArachnidBossAnim.h"
#include "PP_ArachnidBoss.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_ArachnidBoss : public APP_Monster
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APP_ArachnidBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPP_ArachnidBossAnim* MyAnim;

	virtual void ChangeAnimState(AnimType type);

	TSubclassOf<class APP_EffectBase> NormalHitEffect;
	TSubclassOf<class APP_EffectBase> Normal2HitEffect;
	TSubclassOf<class APP_ArachBossSkill1> Skill1Effect;
	TSubclassOf<class APP_DotDamageSkill> Skill2Effect;

	float Skill1CoolAcc;
	float Skill2CoolAcc;
	float Skill3CoolAcc;

	float SkillCoolTime[3];

	UPROPERTY()
		TSubclassOf<class APP_DropItem> Drop_Item;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Attack1Fire();
	void Attack2Fire();
	void Skill1Fire();
	void Skill2Fire(bool right = true);
	void Skill3Fire();
	virtual void Idle()
	{
		Super::Idle();
		isSkillCast = false;
	}
};
