// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "PP_Player.h"
#include "GameFramework/Character.h"
#include "PP_Monster.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_Monster : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPP_MonsterInfo MonsterStatus;

public:
	// Sets default values for this character's properties
	APP_Monster();
	virtual void ChangeAnimState(AnimType type)
	{
		MonsterStatus.AnimState = type;
	}

	virtual void SetTarget(AActor* target)
	{
		m_Target = target;
		ChangeAnimState(AnimType::Idle);
	}

	FPP_MonsterInfo* GetStatus()
	{
		return &MonsterStatus;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	bool isSkillCast;
	UPROPERTY()
	AActor* m_Target;

	class APP_MonsterSpawnPotin* MySpawnPoint;
	
	UPROPERTY()
	bool nonedead;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Idle() { ChangeAnimState(AnimType::Idle); isSkillCast = false; }
	virtual void Guard() { ChangeAnimState(AnimType::Guard); }
	virtual void NormalAttack() { ChangeAnimState(AnimType::Attack1); }
	virtual void NormalAttack2() { ChangeAnimState(AnimType::Attacl2); }
	virtual void Skill1() { ChangeAnimState(AnimType::Skill1); isSkillCast = true; }
	virtual void Skill2() { ChangeAnimState(AnimType::Skill2); isSkillCast = true; }
	virtual void Skill3() { ChangeAnimState(AnimType::Skill3); isSkillCast = true; }
	virtual void Skill4() { ChangeAnimState(AnimType::Skill4); isSkillCast = true; }
	virtual void Skill5() { ChangeAnimState(AnimType::Skill5); isSkillCast = true; }
	virtual void Skill6() { ChangeAnimState(AnimType::Skill6); isSkillCast = true; }
	virtual void Death() { ChangeAnimState(AnimType::Death); SetLifeSpan(3.0);}
	virtual void Reraise() { ChangeAnimState(AnimType::Reraise); }
	virtual void Paralysis() { ChangeAnimState(AnimType::Paralysis); }
	virtual void UsingItem() { ChangeAnimState(AnimType::UsingItem); }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void SetSpawnPoint(APP_MonsterSpawnPotin* SpawnPoint) { MySpawnPoint = SpawnPoint; }

	void SetParalysis()
	{
		MonsterStatus.Paralysis_Acc = 0;
		MonsterStatus.Debuff[0] = true;
		Paralysis();
	}

	void HpUp(int amount);
};
