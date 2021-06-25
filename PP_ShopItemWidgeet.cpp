// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ShopItemWidgeet.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PlayerController.h"
#include "PP_Player.h"
#include "Widgets/SWidget.h"
#include "PP_PlayerController.h"

void UPP_ShopItemWidgeet::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_Icon = Cast<UImage>(GetWidgetFromName(TEXT("Image_Main")));
	m_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Text")));
	m_Price = Cast<UTextBlock>(GetWidgetFromName(TEXT("Item_Price")));
	MyButton = Cast<UButton>(GetWidgetFromName(TEXT("Mybt")));
	//버튼 애드 동작안해서 블루프린트로
	MyButton->OnClicked.AddDynamic(this, &UPP_ShopItemWidgeet::ShopItemClick);
}

void UPP_ShopItemWidgeet::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPP_ShopItemWidgeet::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UPP_ShopItemWidgeet::SetIconTexture(const FString& Path)
{
	UTexture2D* pIconTex = LoadObject<UTexture2D>(nullptr, *Path);

	m_Icon->SetBrushFromTexture(pIconTex);
}

void UPP_ShopItemWidgeet::SetIconTexture(UTexture2D* pTex)
{
	m_Icon->SetBrushFromTexture(pTex);
}

void UPP_ShopItemWidgeet::SetText(const FString& text)
{
	m_Text->SetText(FText::FromString(text));
}

void UPP_ShopItemWidgeet::SetPrice(const FString& text)
{
	m_Price->SetText(FText::FromString(text));
}

void UPP_ShopItemWidgeet::SetData(UPP_ShopItemData* data)
{
	SetText(data->GetDesc());
	SetPrice(data->GetPriceString());
	SetIconTexture(data->GetTextureP());
	MyData = data;
}

void UPP_ShopItemWidgeet::ShopItemClick()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp)
	{
		UPP_GameInstance* MyInstance = Cast<UPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (MyInstance)
		{
			int isMinus = MyInstance->GoldDown(MyData->GetPrice());
			if(isMinus >= 0)
				temp->BuyItem((int)(MyData->GetItemType()));
		}
	}
}