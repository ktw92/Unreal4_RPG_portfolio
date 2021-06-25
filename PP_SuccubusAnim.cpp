// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_SuccubusAnim.h"
#include "PP_Succubus.h"


UPP_SuccubusAnim::UPP_SuccubusAnim()
{

}

void UPP_SuccubusAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPP_SuccubusAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPP_SuccubusAnim::AnimNotify_Attack1()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->Attack1Fire();
}

void UPP_SuccubusAnim::AnimNotify_Skill1()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill1Fire();
}

void UPP_SuccubusAnim::AnimNotify_Skill2()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill2Fire();
}

void UPP_SuccubusAnim::AnimNotify_Skill3()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill3Fire();
}

void UPP_SuccubusAnim::AnimNotify_Skill4()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill4Fire();
}

void UPP_SuccubusAnim::AnimNotify_Skill5()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill5Fire();
}

void UPP_SuccubusAnim::AnimNotify_DeathBoom()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->DeathBoom();
}

void UPP_SuccubusAnim::AnimNotify_Idle()
{
	APP_Succubus* Owner = Cast<APP_Succubus>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}