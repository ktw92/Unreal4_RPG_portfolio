// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_OrcAnim.h"
#include "PP_Orc.h"


UPP_OrcAnim::UPP_OrcAnim()
{

}

void UPP_OrcAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!Owner)
		Owner = Cast<APP_Orc>(TryGetPawnOwner());
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("orc anim owner none")));
}

void UPP_OrcAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPP_OrcAnim::AnimNotify_Attack1()
{
	if (Owner)
		Owner->NormalAttack();
}

void UPP_OrcAnim::AnimNotify_Idle()
{
	if (Owner)
		Owner->Idle();
}

void UPP_OrcAnim::AnimNotify_Skill1Fire()
{
	if (Owner)
		Owner->Skill1Frie();
}

void UPP_OrcAnim::AnimNotify_Skill2Fire()
{
	if (Owner)
		Owner->Skill2Frie();
}