// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_MandrakeAnim.h"
#include "PP_Mandrake.h"


void UPP_MandrakeAnim::AnimNotify_Skill1()
{
	APP_Mandrake* Owner = Cast<APP_Mandrake>(TryGetPawnOwner());
	if (Owner)
		Owner->SkillFire();
}

void UPP_MandrakeAnim::AnimNotify_Idle()
{
	APP_Mandrake* Owner = Cast<APP_Mandrake>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}