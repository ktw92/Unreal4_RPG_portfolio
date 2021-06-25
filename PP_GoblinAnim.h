// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "Animation/AnimInstance.h"
#include "PP_GoblinAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_GoblinAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPP_GoblinAnim();
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
	void AnimNotify_Idle();
};
