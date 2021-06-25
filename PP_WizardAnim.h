// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "Animation/AnimInstance.h"
#include "PP_WizardAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_WizardAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPP_WizardAnim();
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	//애니메이션 전환용 상태변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AnimType Anim_state;

	class APP_Wizard* Owner;

public:
	void ChangeAnimState(AnimType type)
	{
		Anim_state = type;
	}

	UFUNCTION()
		void AnimNotify_NormalAttackFire();
	UFUNCTION()
		void AnimNotify_UsingItemFire();
	UFUNCTION()
		void AnimNotify_Skill1Fire();
	UFUNCTION()
		void AnimNotify_Skill2Fire();
	UFUNCTION()
		void AnimNotify_Skill3Fire();
	UFUNCTION()
		void AnimNotify_Skill4Fire();
	UFUNCTION()
		void AnimNotify_Skill5Fire();
	UFUNCTION()
		void AnimNotify_SkillEnd();
	UFUNCTION()
		void AnimNotify_ReraiseEnd();
};
