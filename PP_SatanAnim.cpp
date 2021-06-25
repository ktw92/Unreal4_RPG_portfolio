// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_SatanAnim.h"
#include "PP_Satan.h"


UPP_SatanAnim::UPP_SatanAnim()
{

}

void UPP_SatanAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPP_SatanAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}


void UPP_SatanAnim::AnimNotify_Attack1()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Attack1Fire();
}

void UPP_SatanAnim::AnimNotify_Skill1()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill1Fire();
}

void UPP_SatanAnim::AnimNotify_Skill2()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill2Fire(true);
}

void UPP_SatanAnim::AnimNotify_Skill2_2()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill2Fire(false);
}

void UPP_SatanAnim::AnimNotify_Skill3()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill3Fire();
}

void UPP_SatanAnim::AnimNotify_Skill4()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill4Fire();
}

void UPP_SatanAnim::AnimNotify_Skill5()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill5Fire();
}

void UPP_SatanAnim::AnimNotify_Skill6()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Skill6Fire();
}

void UPP_SatanAnim::AnimNotify_Idle()
{
	APP_Satan* Owner = Cast<APP_Satan>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}