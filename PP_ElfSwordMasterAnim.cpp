// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ElfSwordMasterAnim.h"
#include "PP_ElfSwordMaster.h"

UPP_ElfSwordMasterAnim::UPP_ElfSwordMasterAnim()
{

}

void UPP_ElfSwordMasterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!Owner)
		Owner = Cast<APP_Player>(TryGetPawnOwner());

	Anim_state = AnimType::Idle;
}

void UPP_ElfSwordMasterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UPP_ElfSwordMasterAnim::ChangeAnimState(AnimType type)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("ElfSwordMasterAnim Change AnimState")));
	Anim_state = type;

}

void UPP_ElfSwordMasterAnim::AnimNotify_NormalAttack()
{

	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if(temp)
			temp->NormalAttack();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_SkillEnd()
{
	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if(temp)
			temp->SetIdle();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill1()
{

	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if(temp)
			temp->Skill1();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill2Start()
{

	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if(temp)
			temp->Skill2Start();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Anim Skill2 Start")));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill2End()
{

	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if(temp)
			temp->Skill2End();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Anim Skill2 end")));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill3()
{

	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		temp->Skill3();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill4Start()
{
	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if (temp)
			temp->Skill4Start();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill4()
{
	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if(temp)
			temp->Skill4();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill5Start()
{
	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if (temp)
			temp->Skill5Start();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_Skill5()
{

	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if (temp)
			temp->Skill5();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_UsingItemFire()
{
	if (Owner)
	{
		APP_ElfSwordMaster* temp = Cast<APP_ElfSwordMaster>(Owner);
		if (temp)
			temp->UsingItemFire();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none elfswordmaster anim owner")));
	}
}

void UPP_ElfSwordMasterAnim::AnimNotify_ReraiseEnd()
{
	if (Owner)
	{
		Owner->ReraiseEnd();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("pristessAnim anim owner")));
	}
}


