// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "PP_ArachBossSkill1.h"
#include "Animation/AnimInstance.h"
#include "PP_ArachBossAnim.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_ArachBossAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPP_ArachBossAnim();
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AnimType MyAnimType;

	UFUNCTION()
	void AnimNotify_Skill1();

	UFUNCTION()
	void AnimNotify_Skill1End();

protected:
	TSubclassOf<class APP_ArachBossSkill1> Sk1;
};
