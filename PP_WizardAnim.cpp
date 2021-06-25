// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_WizardAnim.h"
#include "PP_Wizard.h"

UPP_WizardAnim::UPP_WizardAnim()
{

}

void UPP_WizardAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!Owner)
		Owner = Cast<APP_Wizard>(TryGetPawnOwner());

	Anim_state = AnimType::Idle;
}

void UPP_WizardAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UPP_WizardAnim::AnimNotify_NormalAttackFire()
{
	if (Owner)
	{
		Owner->NormalAttackFire();
	}
}

void UPP_WizardAnim::AnimNotify_UsingItemFire()
{
	if (Owner)
	{
		Owner->UsingItemFire();
	}
}

void UPP_WizardAnim::AnimNotify_Skill1Fire()
{
	if (Owner)
	{
		Owner->Skill1Fire();
	}
}

void UPP_WizardAnim::AnimNotify_Skill2Fire()
{
	if (Owner)
	{
		Owner->Skill2Fire();
	}
}

void UPP_WizardAnim::AnimNotify_Skill3Fire()
{
	if (Owner)
	{
		Owner->Skill3Fire();
	}
}

void UPP_WizardAnim::AnimNotify_Skill4Fire()
{
	if (Owner)
	{
		Owner->Skill4Fire();
	}
}

void UPP_WizardAnim::AnimNotify_Skill5Fire()
{
	if (Owner)
	{
		Owner->Skill5Fire();
	}
}

void UPP_WizardAnim::AnimNotify_SkillEnd()
{
	if (Owner)
	{
		Owner->SetIdle();
	}
}

void UPP_WizardAnim::AnimNotify_ReraiseEnd()
{
	if (Owner)
	{
		Owner->ReraiseEnd();
	}
}