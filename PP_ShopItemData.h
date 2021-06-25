// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "UObject/NoExportTypes.h"
#include "PP_ShopItemData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ELFPALADINTEMPLATE_API UPP_ShopItemData : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		FString Desc;
	UPROPERTY()
		FString Price_String;
	UPROPERTY()
		int32 Price;
	UPROPERTY()
		EquipItemType ItemIndex;
	UPROPERTY()
		UTexture2D* m_IconTexture;
public:
	void SetIconTexture(const FString& Path)
	{
		m_IconTexture = LoadObject<UTexture2D>(nullptr, *Path);
	}

	void SetIconTexture(UTexture2D* pTex)
	{
		m_IconTexture = pTex;
	}

	void SetString(FString str)
	{
		Desc = str;
	}

	void SetPrice(int32 p)
	{
		Price = p;
		Price_String = FString::Printf(TEXT("Prcie : %d"), Price);
	}

	int32 GetPrice()
	{
		return Price;
	}

	FString GetPriceString()
	{
		return Price_String;
	}

	FString GetDesc()
	{
		return Desc;
	}

	void SetItemType(EquipItemType type)
	{
		ItemIndex = type;
	}

	EquipItemType GetItemType()
	{
		return ItemIndex;
	}

	UTexture2D* GetTextureP()
	{
		return m_IconTexture;
	}
};
