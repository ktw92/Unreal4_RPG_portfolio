// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ArachnidBossAnim.h"
#include "PP_ArachnidBoss.h"

UPP_ArachnidBossAnim::UPP_ArachnidBossAnim()
{

}

void UPP_ArachnidBossAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//if (!Owner)
		//Owner = Cast<APP_Goblin>(TryGetPawnOwner());
}

void UPP_ArachnidBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPP_ArachnidBossAnim::AnimNotify_Attack1()
{
	APP_ArachnidBoss* Owner = Cast<APP_ArachnidBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Attack1Fire();
}

void UPP_ArachnidBossAnim::AnimNotify_Attack2()
{
	APP_ArachnidBoss* Owner = Cast<APP_ArachnidBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Attack2Fire();
}

void UPP_ArachnidBossAnim::AnimNotify_Skill1()
{
	APP_ArachnidBoss* Owner = Cast<APP_ArachnidBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill1Fire();
}

void UPP_ArachnidBossAnim::AnimNotify_Skill2()
{
	APP_ArachnidBoss* Owner = Cast<APP_ArachnidBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill2Fire();
}

void UPP_ArachnidBossAnim::AnimNotify_Skill2_2()
{
	APP_ArachnidBoss* Owner = Cast<APP_ArachnidBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill2Fire(false);
}


void UPP_ArachnidBossAnim::AnimNotify_Skill3()
{
	APP_ArachnidBoss* Owner = Cast<APP_ArachnidBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill3Fire();
}

void UPP_ArachnidBossAnim::AnimNotify_Idle()
{
	APP_ArachnidBoss* Owner = Cast<APP_ArachnidBoss>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}