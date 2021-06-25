// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_EquipItemWidgeet.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "PP_EquipItemData.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PlayerController.h"
#include "PP_Player.h"
#include "Widgets/SWidget.h"

void UPP_EquipItemWidgeet::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_Icon = Cast<UImage>(GetWidgetFromName(TEXT("Image_Main")));
	m_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Text")));
	m_Owner = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Owner")));
	MyButton = Cast<UButton>(GetWidgetFromName(TEXT("Mybt")));
	//버튼 애드 동작안해서 블루프린트로 EQItemClick 함수 호출했음 대체 왜?
	MyButton->OnClicked.AddDynamic(this, &UPP_EquipItemWidgeet::EQItemClick);
}

void UPP_EquipItemWidgeet::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPP_EquipItemWidgeet::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UPP_EquipItemWidgeet::SetIconTexture(const FString& Path)
{
	UTexture2D* pIconTex = LoadObject<UTexture2D>(nullptr, *Path);

	m_Icon->SetBrushFromTexture(pIconTex);
}

void UPP_EquipItemWidgeet::SetIconTexture(UTexture2D* pTex)
{
	m_Icon->SetBrushFromTexture(pTex);
}

void UPP_EquipItemWidgeet::SetText(const FString& text)
{
	m_Text->SetText(FText::FromString(text));
}

void UPP_EquipItemWidgeet::SetOwner(const FString& text)
{
	m_Owner->SetText(FText::FromString(text));
}

void UPP_EquipItemWidgeet::SetData(UPP_EquipItemData* data)
{
	SetText(data->GetDesc());
	//SetOwner(data->GetOwner());
	SetIconTexture(data->GetTextureP());
	MyData = data;
}

void UPP_EquipItemWidgeet::EQItemClick()
{
	//오너 텍스트 하려면 귀찮아서공백으로(플레이 지장 없으니까..) 수정 필요

	//기존 아이템 소유자 초기화
	APP_Player* temp_owner = MyData->GetOwnerP();
	if (temp_owner)
	{
		//SetOwner(FString::Printf(TEXT("")));
		temp_owner->SetItem(EquipItemType::none);
	}

	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		APP_Player* temp_player = temp_controller->GetControlCharacter();
		if (temp_player)
		{
			//아이템 소유자 전환
			temp_player->SetItem(MyData->GetMyItem());
			MyData->SetOwnerP(temp_player);
			MyData->SetOwner(temp_player->GetPlayerInfo()->Job);
		}
		//장비창 갱신
		temp_controller->ChangeEquip();
	}
}
