// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "Animation/AnimInstance.h"
#include "PP_MandrakeAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_MandrakeAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
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
		void AnimNotify_Skill1();

	UFUNCTION()
		void AnimNotify_Idle();
};
