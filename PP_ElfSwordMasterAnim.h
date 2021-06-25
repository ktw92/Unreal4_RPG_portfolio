// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "PP_Player.h"
#include "Animation/AnimInstance.h"
#include "PP_ElfSwordMasterAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_ElfSwordMasterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPP_ElfSwordMasterAnim();
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	//애니메이션 전환용 상태변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AnimType Anim_state;

	APP_Player* Owner;

public:
	void ChangeAnimState(AnimType type);
	//{		Anim_state = type;}
	UFUNCTION()
	void AnimNotify_NormalAttack();
	UFUNCTION()
	void AnimNotify_SkillEnd();
	UFUNCTION()
	void AnimNotify_Skill1();
	UFUNCTION()
	void AnimNotify_Skill2Start();
	UFUNCTION()
	void AnimNotify_Skill2End();
	UFUNCTION()
	void AnimNotify_Skill3();
	UFUNCTION()
	void AnimNotify_Skill4();
	UFUNCTION()
	void AnimNotify_Skill4Start();
	UFUNCTION()
	void AnimNotify_Skill5Start();
	UFUNCTION()
	void AnimNotify_Skill5();
	UFUNCTION()
	void AnimNotify_UsingItemFire();
	UFUNCTION()
	void AnimNotify_ReraiseEnd();
};
