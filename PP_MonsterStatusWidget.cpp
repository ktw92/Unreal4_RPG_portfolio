// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_MonsterStatusWidget.h"
#include "PP_PlayerController.h"
#include "PP_Player.h"
#include "PP_Monster.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/SWidget.h"



void UPP_MonsterStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP")));
	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name_textblock")));

}

void UPP_MonsterStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		CharacterController = temp_controller;
	}
	SetHidden();
}

void UPP_MonsterStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//타겟팅 된 몬스터 검색
	APP_Monster* temp_monster = nullptr;
	if (CharacterController)
	{
		APP_Player* temp_player = CharacterController->GetControlCharacter();
		if (temp_player)
		{
			AActor* temp_actor = temp_player->GetTarget();
			if (!temp_actor)
			{
				temp_actor = temp_player->GetSkillTarget();
			}
			if (temp_actor)
			{
				temp_monster = Cast<APP_Monster>(temp_actor);
			}
		}
	}
	else
	{
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			CharacterController = temp_controller;
		}
	}

	if (temp_monster)
	{
		SetVisible();
		SetNameText(temp_monster->GetStatus()->Name);
		float max_hp = temp_monster->GetStatus()->Max_hp;
		float hp = temp_monster->GetStatus()->Hp;
		SetHpPercent(hp / max_hp);
	
		isStart = true;
		isHiddening = false;
		Hiddenacc = 0;
	}
	else
	{
		if (isStart)
		{
			isStart = false;
			SetHpPercent(0);
			SetNameText("target none");
			isHiddening = true;
		}
	}

	//타겟없어지면 일정시간 후 제거
	if (isHiddening)
	{
		Hiddenacc += InDeltaTime;
		if (Hiddenacc >= 1.2f)
		{
			Hiddenacc = 0;
			SetHidden();
			isHiddening = false;
		}
	}
}

void UPP_MonsterStatusWidget::SetNameText(const FString& text)
{
	NameText->SetText(FText::FromString(text));
}

void UPP_MonsterStatusWidget::SetHidden()
{
	FLinearColor temp;
	temp.R = 0;
	temp.G = 0;
	temp.B = 0;
	temp.A = 0;
	this->SetColorAndOpacity(temp);
}

void UPP_MonsterStatusWidget::SetVisible()
{
	FLinearColor temp;
	temp.R = 1;
	temp.G = 1;
	temp.B = 1;
	temp.A = 1;
	this->SetColorAndOpacity(temp);
}

