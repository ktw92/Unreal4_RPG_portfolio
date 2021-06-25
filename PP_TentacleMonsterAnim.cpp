// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_TentacleMonsterAnim.h"
#include "PP_TectacleMonster.h"


void UPP_TentacleMonsterAnim::AnimNotify_Attack1()
{
	APP_TectacleMonster* Owner = Cast<APP_TectacleMonster>(TryGetPawnOwner());
	if (Owner)
	{
		Owner->NormalAttack();
	}
}

void UPP_TentacleMonsterAnim::AnimNotify_Idle()
{
	APP_TectacleMonster* Owner = Cast<APP_TectacleMonster>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}

void UPP_TentacleMonsterAnim::AnimNotify_DeathBoom()
{
	APP_TectacleMonster* Owner = Cast<APP_TectacleMonster>(TryGetPawnOwner());
	if (Owner)
		Owner->DeathBoom();
}