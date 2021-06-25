// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_LastBossAnim.h"
#include "PP_LastBoss.h"

UPP_LastBossAnim::UPP_LastBossAnim()
{
	
}

void UPP_LastBossAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPP_LastBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}


void UPP_LastBossAnim::AnimNotify_Attack1()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Attack1Fire();
}

void UPP_LastBossAnim::AnimNotify_Skill1()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill1Fire();
}

void UPP_LastBossAnim::AnimNotify_Skill2()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill2Fire(true);
}

void UPP_LastBossAnim::AnimNotify_Skill2_2()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill2Fire(false);
}

void UPP_LastBossAnim::AnimNotify_Skill3()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill3Fire();
}

void UPP_LastBossAnim::AnimNotify_Skill4()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill4Fire();
}

void UPP_LastBossAnim::AnimNotify_Skill5()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill5Fire();
}

void UPP_LastBossAnim::AnimNotify_Skill6()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill6Fire();
}

void UPP_LastBossAnim::AnimNotify_Idle()
{
	APP_LastBoss* Owner = Cast<APP_LastBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}

