// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GameInstance.h"
#include "PP_EquipItemData.h"

UPP_GameInstance::UPP_GameInstance()
{
	isSave = false;
	Gold = 1000;
}

void UPP_GameInstance::SetPlayerInfo(FPP_PlayerInfo* data, int index)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("index : %d, hp : %d"), index, data->Hp));
	CharacterArray[index].Attack_range = data->Attack_range;
	CharacterArray[index].Attack[0] = data->Attack[0];
	CharacterArray[index].Defense[0] = data->Defense[0];
	CharacterArray[index].Max_hp = data->Max_hp;
	CharacterArray[index].Hp = data->Hp;
	CharacterArray[index].Max_mp = data->Max_mp;
	CharacterArray[index].Mp = data->Mp;
	CharacterArray[index].Exp = data->Exp;
}

void UPP_GameInstance::AddItem(UPP_EquipItemData* data)
{
	if (data->GetMyItem() == EquipItemType::Power)
	{
		UPP_EquipItemData* Data1 = NewObject<UPP_EquipItemData>(this,
			UPP_EquipItemData::StaticClass());
		Data1->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_1.EquipItem_1'"));
		Data1->SetString(FString("attack +2000"));
		Data1->SetOwner(FString(""));
		Data1->SetMyItem(EquipItemType::Power);
		EquipItemDatas.Add(Data1);
	}
	else if (data->GetMyItem() == EquipItemType::Defense)
	{
		UPP_EquipItemData* Data2 = NewObject<UPP_EquipItemData>(this,
			UPP_EquipItemData::StaticClass());
		Data2->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_2.EquipItem_2'"));
		Data2->SetString(FString("defense +2000"));
		Data2->SetOwner(FString(""));
		Data2->SetMyItem(EquipItemType::Defense);
		EquipItemDatas.Add(Data2);
	}
	else if (data->GetMyItem() == EquipItemType::SoftPower)
	{
		UPP_EquipItemData* Data3 = NewObject<UPP_EquipItemData>(this,
			UPP_EquipItemData::StaticClass());
		Data3->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_3.EquipItem_3'"));
		Data3->SetString(FString("attack + 1000"));
		Data3->SetOwner(FString(""));
		Data3->SetMyItem(EquipItemType::SoftPower);
		EquipItemDatas.Add(Data3);
	}
	else if (data->GetMyItem() == EquipItemType::SoftDefense)
	{
		UPP_EquipItemData* Data3 = NewObject<UPP_EquipItemData>(this,
			UPP_EquipItemData::StaticClass());
		Data3->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_4.EquipItem_4'"));
		Data3->SetString(FString("defense + 1000"));
		Data3->SetOwner(FString(""));
		Data3->SetMyItem(EquipItemType::SoftDefense);
		EquipItemDatas.Add(Data3);
	}
	else if (data->GetMyItem() == EquipItemType::Mix)
	{
		UPP_EquipItemData* Data3 = NewObject<UPP_EquipItemData>(this,
			UPP_EquipItemData::StaticClass());
		Data3->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_5.EquipItem_5'"));
		Data3->SetString(FString("Attack and Defense + 1500"));
		Data3->SetOwner(FString(""));
		Data3->SetMyItem(EquipItemType::Mix);
		EquipItemDatas.Add(Data3);
	}
	else
		;
}

void UPP_GameInstance::ItemBackUp()
{
	EquipItemArraytemp.Reset();
	for (auto& item : EquipItemDatas)
	{
		EquipItemArraytemp.Add(item->GetMyItem());
	}
}

void UPP_GameInstance::ItemOwnerBackUp()
{
	EquipItemOwner.Reset();
	for (auto& item : EquipItemDatas)
	{
		EquipItemOwner.Add(item->GetOwner());
	}
}

void UPP_GameInstance::ItemRestore()
{
	EquipItemDatas.Reset();
	int i = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("")));
	for (auto& item : EquipItemArraytemp)
	{
		if (item == EquipItemType::Power)
		{
			UPP_EquipItemData* Data1 = NewObject<UPP_EquipItemData>(this,
				UPP_EquipItemData::StaticClass());
			Data1->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_1.EquipItem_1'"));
			Data1->SetString(FString("attack + 2000"));
			Data1->SetOwner(EquipItemOwner[i]);
			//Data1->SetOwner(FString(""));
			Data1->SetMyItem(EquipItemType::Power);
			EquipItemDatas.Add(Data1);
		}
		else if (item == EquipItemType::Defense)
		{
			UPP_EquipItemData* Data2 = NewObject<UPP_EquipItemData>(this,
				UPP_EquipItemData::StaticClass());
			Data2->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_2.EquipItem_2'"));
			Data2->SetString(FString("defense + 2000"));
			Data2->SetOwner(EquipItemOwner[i]);
			//Data2->SetOwner(FString(""));
			Data2->SetMyItem(EquipItemType::Defense);
			EquipItemDatas.Add(Data2);
		}
		else if (item == EquipItemType::SoftPower)
		{
			UPP_EquipItemData* Data3 = NewObject<UPP_EquipItemData>(this,
				UPP_EquipItemData::StaticClass());
			Data3->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_3.EquipItem_3'"));
			Data3->SetString(FString("attack + 1000"));
			Data3->SetOwner(EquipItemOwner[i]);
			Data3->SetMyItem(EquipItemType::SoftPower);
			EquipItemDatas.Add(Data3);
		}
		else if (item == EquipItemType::SoftDefense)
		{
			UPP_EquipItemData* Data3 = NewObject<UPP_EquipItemData>(this,
				UPP_EquipItemData::StaticClass());
			Data3->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_4.EquipItem_4'"));
			Data3->SetString(FString("defense + 1000"));
			Data3->SetOwner(EquipItemOwner[i]);
			Data3->SetMyItem(EquipItemType::SoftDefense);
			EquipItemDatas.Add(Data3);
		}
		else if (item == EquipItemType::Mix)
		{
			UPP_EquipItemData* Data3 = NewObject<UPP_EquipItemData>(this,
				UPP_EquipItemData::StaticClass());
			Data3->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_5.EquipItem_5'"));
			Data3->SetString(FString("Attack and Defense + 1500"));
			Data3->SetOwner(EquipItemOwner[i]);
			Data3->SetMyItem(EquipItemType::Mix);
			EquipItemDatas.Add(Data3);
		}
		else
			;
		i++;
	}
}