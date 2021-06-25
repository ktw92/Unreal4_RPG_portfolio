// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "Animation/AnimInstance.h"
#include "PP_BoomspiderAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_BoomspiderAnim : public UAnimInstance
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
		void AnimNotify_Attack1();

	UFUNCTION()
		void AnimNotify_Idle();

	UFUNCTION()
		void AnimNotify_DeathBoom();
};
