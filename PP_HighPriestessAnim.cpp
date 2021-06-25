// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_HighPriestessAnim.h"
#include "PP_HighPriestess.h"

UPP_HighPriestessAnim::UPP_HighPriestessAnim()
{

}

void UPP_HighPriestessAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!Owner)
		Owner = Cast<APP_HighPriestess>(TryGetPawnOwner());

	Anim_state = AnimType::Idle;
}

void UPP_HighPriestessAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UPP_HighPriestessAnim::AnimNotify_NormalAttackFire()
{
	if (Owner)
	{
		Owner->NormalAttackFire();
	}
}

void UPP_HighPriestessAnim::AnimNotify_UsingItemFire()
{
	if (Owner)
	{
		Owner->UsingItemFire();
	}
}

void UPP_HighPriestessAnim::AnimNotify_Skill1Fire()
{
	if (Owner)
	{
		Owner->Skill1Fire();
	}
}

void UPP_HighPriestessAnim::AnimNotify_Skill2Fire()
{
	if (Owner)
	{
		Owner->Skill2Fire();
	}
}

void UPP_HighPriestessAnim::AnimNotify_Skill3Fire()
{
	if (Owner)
	{
		Owner->Skill3Fire();
	}
}

void UPP_HighPriestessAnim::AnimNotify_Skill4Fire()
{
	if (Owner)
	{
		Owner->Skill4Fire();
	}
}

void UPP_HighPriestessAnim::AnimNotify_Skill5Fire()
{
	if (Owner)
	{
		Owner->Skill5Fire();
	}
}

void UPP_HighPriestessAnim::AnimNotify_SkillEnd()
{
	if (Owner)
	{
		Owner->SetIdle();
	}
}

void UPP_HighPriestessAnim::AnimNotify_ReraiseEnd()
{
	if (Owner)
	{
		Owner->ReraiseEnd();
	}
}

