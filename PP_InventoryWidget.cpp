// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_InventoryWidget.h"
#include "Components/Button.h"
#include "Components/TileView.h"
#include "Components/ListView.h"
#include "PP_EquipItemData.h"
#include "Widgets/SWidget.h"


void UPP_InventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	EquipItemButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Equip")));
	//EquipItemButton->OnClicked.AddDynamic(this, &UPP_InventoryWidget::EquipBT);
	//ConsumItemButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Consum")));
	//ConsumItemButton->OnClicked.AddDynamic(this, &UPP_InventoryWidget::ConsumBT);
	//m_ConsumTileView = Cast<UTileView>(GetWidgetFromName(TEXT("ConsumView")));
	m_EquipListView = Cast<UListView>(GetWidgetFromName(TEXT("EquipView")));
}

void UPP_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPP_InventoryWidget::EquipBT()
{
	//m_ConsumTileView->SetVisibility(ESlateVisibility::Hidden);
	m_EquipListView->SetVisibility(ESlateVisibility::Visible);
}

void UPP_InventoryWidget::ConsumBT()
{
	//m_ConsumTileView->SetVisibility(ESlateVisibility::Visible);
	m_EquipListView->SetVisibility(ESlateVisibility::Hidden);
}

void UPP_InventoryWidget::AddEquipItem(class UPP_EquipItemData* item)
{
	m_EquipListView->AddItem(item);
}

void UPP_InventoryWidget::ResetEquipitem()
{
	m_EquipListView->ClearListItems();
}
