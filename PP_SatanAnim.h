// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "Animation/AnimInstance.h"
#include "PP_SatanAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_SatanAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPP_SatanAnim();
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
	void SetAnimState(AnimType type)
	{
		MyAnimType = type;
	}
	AnimType GetAnimState()
	{
		return MyAnimType;
	}
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AnimType MyAnimType;

public:
	UFUNCTION()
		void AnimNotify_Attack1();
	UFUNCTION()
		void AnimNotify_Skill1();
	UFUNCTION()
		void AnimNotify_Skill2();
	UFUNCTION()
		void AnimNotify_Skill2_2();
	UFUNCTION()
		void AnimNotify_Skill3();
	UFUNCTION()
		void AnimNotify_Skill4();
	UFUNCTION()
		void AnimNotify_Skill5();
	UFUNCTION()
		void AnimNotify_Skill6();
	UFUNCTION()
		void AnimNotify_Idle();
};
