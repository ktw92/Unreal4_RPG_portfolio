// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "UObject/NoExportTypes.h"
#include "PP_EquipItemData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ELFPALADINTEMPLATE_API UPP_EquipItemData : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FString Desc;
	UPROPERTY()
	FString Owner;
	UPROPERTY()
	EquipItemType MyType;
	UPROPERTY()
	UTexture2D* m_IconTexture;
	UPROPERTY()
	class APP_Player* Owner_pointer;

public:
	UTexture2D* GetTextureP()
	{
		return m_IconTexture;
	}
	FString GetDesc()
	{
		return Desc;
	}

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

	void SetOwner(FString charjob)
	{
		Owner = charjob;
	}

	void SetMyItem(EquipItemType type)
	{
		MyType = type;
	}

	EquipItemType GetMyItem()
	{
		return MyType;
	}

	FString GetOwner()
	{
		return Owner;
	}

	void SetOwnerP(class APP_Player* p)
	{
		Owner_pointer = p;
	}

	class APP_Player* GetOwnerP()
	{
		return Owner_pointer;
	}
	
};
