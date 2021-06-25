// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_Monster.h"
#include "PP_SatanAnim.h"
#include "PP_Satan.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_Satan : public APP_Monster
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APP_Satan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float SkillCoolTime[6];
	virtual void ChangeAnimState(AnimType type);

	TSubclassOf<class APP_ProjectileAttack> NormalAttackProj;

	TSubclassOf<class APP_DotDamageSkill> Skill1Effect;
	TSubclassOf<class APP_EffectBase> Skill1_1HitEffect;
	TSubclassOf<class APP_DotHealSkill> Skill1_2Effect;
	TSubclassOf<class APP_EffectBase> Skill1_2HitEffect;

	TSubclassOf<class APP_CircleMoveAttack> Skill2Effect;
	TSubclassOf<class APP_EffectBase> Skill12HitEffect;

	TSubclassOf<class APP_ProjectileBoom> Skill3Effect;

	TSubclassOf<class APP_ProjectileBoom> Skill4Effect;

	TSubclassOf<class APP_ProjectileBoom> Skill5Effect;

	UPP_SatanAnim* MyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Skill5dsit;

	UPROPERTY()
		TSubclassOf<class APP_DropItem> Drop_Item;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack1Fire();
	void Skill1Fire();
	void Skill2Fire(bool isR);
	void Skill3Fire();
	void Skill4Fire();
	void Skill5Fire();
	void Skill6Fire();
};
