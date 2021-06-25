// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "Animation/AnimInstance.h"
#include "PP_OrcAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_OrcAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPP_OrcAnim();
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
	APP_Orc* Owner;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		AnimType MyAnimType;

public:
	UFUNCTION()
		void AnimNotify_Attack1();

	UFUNCTION()
		void AnimNotify_Idle();

	UFUNCTION()
		void AnimNotify_Skill1Fire();
	UFUNCTION()
		void AnimNotify_Skill2Fire();
};
