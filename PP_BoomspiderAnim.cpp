// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_BoomspiderAnim.h"
#include "PP_BoomSpider.h"


void UPP_BoomspiderAnim::AnimNotify_Attack1()
{
	APP_BoomSpider* Owner = Cast<APP_BoomSpider>(TryGetPawnOwner());
	if (Owner)
		Owner->NormalAttack();
}

void UPP_BoomspiderAnim::AnimNotify_Idle()
{
	APP_BoomSpider* Owner = Cast<APP_BoomSpider>(TryGetPawnOwner());
	if (Owner)
		Owner->Idle();
}

void UPP_BoomspiderAnim::AnimNotify_DeathBoom()
{
	APP_BoomSpider* Owner = Cast<APP_BoomSpider>(TryGetPawnOwner());
	if (Owner)
		Owner->DeathBoom();
}