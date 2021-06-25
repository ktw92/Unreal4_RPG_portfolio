// Fill out your copyright notice in the Description page of Project Settings.

#include "PP_GoblinAnim.h"
#include "PP_Goblin.h"

UPP_GoblinAnim::UPP_GoblinAnim()
{
	
}

void UPP_GoblinAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//if (!Owner)
		//Owner = Cast<APP_Goblin>(TryGetPawnOwner());
}

void UPP_GoblinAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPP_GoblinAnim::AnimNotify_Attack1()
{
	APP_Goblin* Owner = Cast<APP_Goblin>(TryGetPawnOwner());
	if (Owner)
		Owner->NormalAttack();
	//else
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("goblin anim owner none")));
}

void UPP_GoblinAnim::AnimNotify_Idle()
{
	APP_Goblin* Owner = Cast<APP_Goblin>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
	//else
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("goblin anim owner none")));
}