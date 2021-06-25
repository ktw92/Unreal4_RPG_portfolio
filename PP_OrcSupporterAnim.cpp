// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_OrcSupporterAnim.h"
#include "PP_OrcSupporter.h"

UPP_OrcSupporterAnim::UPP_OrcSupporterAnim()
{

}

void UPP_OrcSupporterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!Owner)
		Owner = Cast<APP_OrcSupporter>(TryGetPawnOwner());
}

void UPP_OrcSupporterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPP_OrcSupporterAnim::AnimNotify_Attack1()
{
	if (Owner)
		Owner->NormalAttackFire();
}

void UPP_OrcSupporterAnim::AnimNotify_Skill1()
{
	if (Owner)
		Owner->Skill1Fire();
}

void UPP_OrcSupporterAnim::AnimNotify_Idle()
{
	if (Owner)
		Owner->Idle();
}