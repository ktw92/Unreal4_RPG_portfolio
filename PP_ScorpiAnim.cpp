// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ScorpiAnim.h"
#include "PP_Scorpi.h"


void UPP_ScorpiAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPP_ScorpiAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPP_ScorpiAnim::AnimNotify_Attack1()
{
	APP_Scorpi* Owner = Cast<APP_Scorpi>(TryGetPawnOwner());
	if (Owner)
		Owner->NormalAttack();
}

void UPP_ScorpiAnim::AnimNotify_Idle()
{
	APP_Scorpi* Owner = Cast<APP_Scorpi>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}