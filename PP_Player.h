// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "Camera/CameraComponent.h"
#include "PP_EffectBase.h"
#include "PP_PlayerPortraitWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "PP_Player.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APP_Player();

	void CameraTurn(float fScale);
	void CameraZoom(float fScale);
	void CameraLookUp(float fScale);

	//void AIOnOff(bool on);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPP_PlayerInfo PlayerInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;

	TSubclassOf<class APP_EffectBase> NormalHitEffect;
	TSubclassOf<class APP_EffectBase> LevelUpEffect;

	TSubclassOf<class APP_EffectBase> Poison;
	TSubclassOf<class APP_EffectBase> HpEffect;
	TSubclassOf<class APP_EffectBase> MpEffect;
	TSubclassOf<class APP_EffectBase> DebufRecoverEffect;
	TSubclassOf<class APP_EffectBase> ReraiseEffect;


	FVector MoveGoal;
	AActor* TargetedMonster;
	AActor* SkillTarget;
	bool isGuard;
	bool isMoving;
	bool isAutoBattle;
	bool isSKill;
	bool isUsingItem;
	bool isAssemble;
	ConsumeItemType ItemType;
	APP_Player* ItemTarget;
	float yaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPP_PlayerPortraitWidget* MyPortraitWidget;
	//TSubclassOf<AController> BT;

	//virtual void UseItemFire();

	int TargetSkillNum;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FPP_PlayerInfo* GetPlayerInfo()
	{
		return &PlayerInfo;
	}

	virtual void ChangeAnimState(AnimType type)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("p chage %d "), (int)(type)));
		PlayerInfo.AnimState = type;
	}

	void TargetFree()
	{
		TargetedMonster = nullptr;
	}

	bool IsSkill()
	{
		return isSKill;
	}

	void SetSKillTarget(AActor* target, int targetskillnum)
	{
		SkillTarget = target;
		TargetSkillNum = targetskillnum;
	}

	AActor* GetSkillTarget()
	{
		return SkillTarget;
	}

	void SKillTargetFree()
	{
		SkillTarget = nullptr;
		isSKill = false;
		TargetSkillNum = 0;
	}

	void ItemTargetFree()
	{
		isUsingItem = false;
		ItemTarget = nullptr;
	}

	AActor* GetTarget()
	{
		return TargetedMonster;
	}
	UCameraComponent* GetCamera()
	{
		return m_Camera;
	}

	void multipleAttack(int amount, int num = 0)
	{
		PlayerInfo.Attack[num] *= amount;
	}
	void divideAttack(int amount, int num = 0)
	{
		PlayerInfo.Attack[num] /= amount;
	}
	void multipleDefense(int amount, int num = 0)
	{
		PlayerInfo.Defense[num] *= amount;
	}
	void divideDefense(int amount, int num = 0)
	{
		PlayerInfo.Defense[num] /= amount;
	}
	void SetItem(EquipItemType type);
	void RemoveEquipItem();
	

	virtual void SimpleMove(FVector roc);
	virtual void SimpleStop();
	virtual void Guard(bool isParty = false, bool isOn = true);
	virtual void SetTargetMonster(AActor* target);
	virtual void SetIdle() { ChangeAnimState(AnimType::Idle); SKillTargetFree(); ItemTargetFree(); isSKill = false; TargetSkillNum = 0; }
	virtual void NormalAttack() {  ChangeAnimState(AnimType::Attack1); }//if (PlayerInfo.Skill_cooltimeAcc[0] > 4.9f) 
	virtual void Skill1() { ChangeAnimState(AnimType::Skill1); isSKill = true;  PlayerInfo.Skill_cooltimeAcc[0] = 0; }
	virtual void Skill2() { ChangeAnimState(AnimType::Skill2); isSKill = true;  PlayerInfo.Skill_cooltimeAcc[1] = 0; }
	virtual void Skill3() { ChangeAnimState(AnimType::Skill3);  isSKill = true;  PlayerInfo.Skill_cooltimeAcc[2] = 0; }
	virtual void Skill4() { ChangeAnimState(AnimType::Skill4);  isSKill = true; PlayerInfo.Skill_cooltimeAcc[3] = 0; }
	virtual void Skill5() { ChangeAnimState(AnimType::Skill5);  isSKill = true; PlayerInfo.Skill_cooltimeAcc[4] = 0; }
	virtual void Death() { ChangeAnimState(AnimType::Death); PlayerInfo.Debuff[0] = PlayerInfo.Debuff[1] = false; }
	virtual void Reraise()	{ ChangeAnimState(AnimType::Reraise);	}
	virtual void ReraiseEnd() { SetIdle(); PlayerInfo.isDead = false; }
	virtual void Paralysis()
	{
		if(PlayerInfo.Hp>0)
			ChangeAnimState(AnimType::Paralysis);
		GetController()->StopMovement();
		if (MyPortraitWidget)
		{
			MyPortraitWidget->ReSetDenuff(0);
			MyPortraitWidget->SetDeBuff(0, ParalysisTime);
		}
	}
	virtual void UsingItem(ConsumeItemType type, APP_Player* target) { ChangeAnimState(AnimType::UsingItem); isSKill = false; ItemType = type; ItemTarget = target; isUsingItem = true; }
	virtual void UsingItemFire();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void SetAutoBattleOnOff(bool isParty = false, bool isOn = true);

	virtual bool UseMp(int amount);
	bool HpUp(int amount);
	bool HpDown(int amount);
	bool MpUp(int amount);

	FRotator CameraRot;

	void SetAssemble(bool order)
	{
		isAssemble = order;
	}
	
	virtual FVector GetMeshForwardVector();

	virtual void ObtainExp(int amount);

	virtual void LevelUP();

	virtual void SetPoison();

	UPP_PlayerPortraitWidget* GetPortrait()
	{
		return MyPortraitWidget;
	}
};
