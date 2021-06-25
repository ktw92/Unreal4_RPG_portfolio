// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "PP_Player.h"
#include "PP_Weapon.h"
#include "PP_HighPriestessAnim.h"
#include "PP_HighPriestess.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_HighPriestess : public APP_Player
{
	GENERATED_BODY()
public:
	APP_HighPriestess();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPP_HighPriestessAnim* MyAnim;

	virtual void ChangeAnimState(AnimType type) override
	{
		Super::ChangeAnimState(type);
		if (MyAnim)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("highp chage %d "), (int)(type)));
			//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("Change")));
			MyAnim->ChangeAnimState(type);
		}
	//	else
			//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("none anim")));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		APP_Weapon* HighPriestessWeapon;
	TSubclassOf<class APP_Weapon> WeaponWithEffect;


	TSubclassOf<class APP_ProjectileAttack> NormalAttackProj;


	TSubclassOf<class APP_EffectBase> Skill1Effect;
	TSubclassOf<class APP_EffectBase> Skill2Effect;
	TSubclassOf<class APP_EffectBase> Skill3Effect;
	TSubclassOf<class APP_EffectBase> Skill4Effect;
	TSubclassOf<class APP_EffectBase> Skill5Effect;


public:
	virtual void SetIdle();
	virtual void SimpleMove(FVector roc);
	virtual void Guard(bool isParty = false, bool isOn = true);
	virtual void NormalAttack();
	void NormalAttackFire();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();
	virtual void Skill5();
	virtual void Death();
	virtual void Reraise();
	virtual void Paralysis();
	virtual void UsingItem(ConsumeItemType type, APP_Player* target);

	void Skill1Fire();
	void Skill2Fire();
	void Skill3Fire();
	void Skill4Fire();
	void Skill5Fire();
	virtual void LevelUP();
};
