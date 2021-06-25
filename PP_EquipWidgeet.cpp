// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_EquipWidgeet.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Widgets/SWidget.h"

void UPP_EquipWidgeet::NativePreConstruct()
{
	Super::NativePreConstruct();
	Portrait = Cast<UImage>(GetWidgetFromName(TEXT("Portrait_Img")));
	Item = Cast<UImage>(GetWidgetFromName(TEXT("Item_Img")));
	//Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_1.EquipItem_1'
	//Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_2.EquipItem_2'
	//Texture2D'/Game/0_PP/PP_UI/Equip/1p.1p'
	//Texture2D'/Game/0_PP/PP_UI/Equip/2p.2p'
	//Texture2D'/Game/0_PP/PP_UI/Equip/3p.3p'
	// LoadTextureFromPath(TEXT("Texture2D'/Game/0_My/CharacterSelect/UI/ElfPaladinTemplate.ElfPaladinTemplate'"));
	Portrait_texture[0] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/1p.1p'"));
	Portrait_texture[1] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/2p.2p'"));
	Portrait_texture[2] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/3p.3p'"));
	Item_texture[0] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/alpha.alpha'"));
	Item_texture[1] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_1.EquipItem_1'"));
	Item_texture[2] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_2.EquipItem_2'"));
	Item_texture[3] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_3.EquipItem_3'"));
	Item_texture[4] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_4.EquipItem_4'"));
	Item_texture[5] = LoadTextureFromPath(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_5.EquipItem_5'"));
	Portrait->SetBrushFromTexture(Portrait_texture[1]);
	Item->SetBrushFromTexture(Item_texture[1]);
}

UTexture2D* UPP_EquipWidgeet::LoadTextureFromPath(const FString& Path)
{
	UTexture2D* sprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));
	return sprite;
}

void UPP_EquipWidgeet::SetImageAndVisible(int portnum, int itmnum, bool isVisible)
{
	if (isVisible)
		this->SetVisibility(ESlateVisibility::Visible);
	else
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (0 <= portnum && portnum <= 2)
		Portrait->SetBrushFromTexture(Portrait_texture[portnum]);
	if (0 <= itmnum && itmnum <= 5)
		Item->SetBrushFromTexture(Item_texture[itmnum]);
}