// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_Monster.h"
#include "PP_Orc.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_Orc : public APP_Monster
{
	GENERATED_BODY()
public:
	APP_Orc();
protected:
	virtual void BeginPlay() override;
	class UPP_OrcAnim* MyAnim;
	virtual void ChangeAnimState(AnimType type);
	TSubclassOf<class APP_EffectBase> NormalHitEffect;
	TSubclassOf<class APP_EffectBase> Sk1_Effect;
	UPROPERTY()
	bool isSkill1;
	UPROPERTY()
	float SkillCoolTime[2];

	//UPROPERTY()
	//bool isSkillCast;

	class APP_Weapon* OrcWeapon;
	TSubclassOf<class APP_Weapon> WeaponWithEffect;
	UPROPERTY()
	TSubclassOf<class APP_DropItem> Drop_Item;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Idle();
	virtual void NormalAttack();
	virtual void Death();
	virtual void Paralysis();
	virtual void Skill1() { isSkill1 = true; Super::Skill1();  GetController()->StopMovement();}
	virtual void Skill2() { Super::Skill2();  GetController()->StopMovement(); }
	virtual void Skill1Frie();
	virtual void Skill2Frie();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
};
