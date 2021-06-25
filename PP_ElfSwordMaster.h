// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "PP_Player.h"
#include "PP_Weapon.h"
#include "PP_Monster.h"
#include "PP_ElfSwordMasterAnim.h"
#include "PP_ElfSwordMaster.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_ElfSwordMaster : public APP_Player
{
	GENERATED_BODY()
public:
	APP_ElfSwordMaster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPP_ElfSwordMasterAnim* MyAnim;

	virtual void ChangeAnimState(AnimType type) override
	{
		Super::ChangeAnimState(type);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("ElfSwordMaster Change AnimState")));
		if (MyAnim)
			MyAnim->ChangeAnimState(type);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		APP_Weapon* ElfWeapon;
	TSubclassOf<class APP_Weapon> WeaponWithEffect;

	bool isSkill2;
	int SKill2_Damage;
	TSubclassOf<class APP_EffectBase> Skill2HitEffect;

	bool isSkill3;
	float Skill3Time;
	float Skill3Acc;
	int IntSkill3Acc;

	TSubclassOf<class APP_EffectBase> Skill4HitEffect;

	TSubclassOf<class APP_EffectBase> Skill5HitEffect;



public:
	virtual void SimpleMove(FVector roc);
	virtual void SetIdle();
	virtual void Guard(bool isParty = false, bool isOn = true);
	virtual void NormalAttack();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4Start();
	virtual void Skill4();
	virtual void Skill5Start();
	virtual void Skill5();
	virtual void Death();
	virtual void Reraise();
	virtual void Paralysis();
	virtual void UsingItem(ConsumeItemType type, APP_Player* target);
	void ChangeSkill1() { Super::Skill1(); };
	void ChangeSkill2() { Super::Skill2(); };
	void ChangeSkill3() { Super::Skill3(); };
	void ChangeSkill4() { Super::Skill4(); };
	void ChangeSkill5() { Super::Skill5(); };

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	void Skill2Start();
	void Skill2End();
	virtual void LevelUP();
};
