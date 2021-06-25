// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ShopWidget.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Components/ListView.h"
#include "PP_ShopItemData.h"
#include "Widgets/SWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPP_ShopWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	XButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_X")));
	XButton->OnClicked.AddDynamic(this, &UPP_ShopWidget::XBT);
	ShopListView = Cast<UListView>(GetWidgetFromName(TEXT("ShopView")));
	MyGold = Cast<UTextBlock>(GetWidgetFromName(TEXT("Gold")));
}

void UPP_ShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPP_ShopWidget::AddItem(class UPP_ShopItemData* item)
{
	ShopListView->AddItem(item);
}

void UPP_ShopWidget::ResetEquipitem()
{
	ShopListView->ClearListItems();
}

void UPP_ShopWidget::XBT()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPP_ShopWidget::OpenShop()
{
	if(SaleItem1)
		ResetEquipitem();

	SaleItem1 = NewObject<UPP_ShopItemData>(this,
		UPP_ShopItemData::StaticClass());
	SaleItem1->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_4.EquipItem_4'"));
	SaleItem1->SetString(FString("defense + 1000"));
	SaleItem1->SetPrice(1000);
	SaleItem1->SetItemType(EquipItemType::SoftDefense);
	AddItem(SaleItem1);


	SaleItem2 = NewObject<UPP_ShopItemData>(this,
		UPP_ShopItemData::StaticClass());
	SaleItem2->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_3.EquipItem_3'"));
	SaleItem2->SetString(FString("attack + 1000"));
	SaleItem2->SetPrice(1000);
	SaleItem2->SetItemType(EquipItemType::SoftPower);
	AddItem(SaleItem2);
	SetGold();
}

void UPP_ShopWidget::SetGold()
{
	UPP_GameInstance* MyInstance = Cast<UPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyInstance)
	{
		int gold = MyInstance->GoldUp(0);
		GoldString = FString::Printf(TEXT("Gold : %d"), gold);
		MyGold->SetText(FText::FromString(GoldString));
	}
}