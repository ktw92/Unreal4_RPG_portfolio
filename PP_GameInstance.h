// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PP_GameInstance.generated.h"


/**
 * 
 */

#define ParalysisTime 5.0
#define PoisonTime 7.0

//장비아이템타입
UENUM(BlueprintType)
enum class EquipItemType : uint8
{
	none,
	Power,
	Defense,
	SoftPower,
	SoftDefense,
	Mix
};

UENUM(BlueprintType)
enum class MonsterType : uint8
{
	none,
	Goblin,
	Orc,
	Scorpi,
	BoomSpider,
	Arachne,
	TectacleMonster,
	Succbus,
	Satan,
	Lavos
};

//소비아이템 종류
UENUM(BlueprintType)
enum class ConsumeItemType : uint8
{
	Hp_potion,
	Mp_potion,
	Debuff_recover,
	Resurrectioner
};

//애니메이션 상태
UENUM(BlueprintType)
enum class AnimType : uint8
{
	Idle,
	Move,
	Guard,
	Attack1,
	Attacl2,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	Skill5,
	Skill6,
	Damnaged,
	Paralysis,
	Death,
	Reraise,
	UsingItem
};

UENUM(BlueprintType)
enum class PlayerAIType : uint8
{
	AutoBattle,
	Guard,
	NormalMove,
	TacticalMove,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	Skill5
};

USTRUCT(BlueprintType)
struct FPP_MonsterInfo //: public FTableRowBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Name;

	FName Name_name;

	MonsterType Type;
	
	int32 Attack[2];

	int32 Defense[2];

	int32 Max_hp;

	int32 Hp;

	bool Debuff[2];

	float Paralysis_Acc;

	int32 Exp;

	float Attack_range;

	float Skill_cooltime[6];

	AnimType AnimState;

};


USTRUCT(BlueprintType)
struct FPP_PlayerInfo// : public FTableRowBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString	Job;
	
	int32 Level;
	
	int32 Attack[2];

	int32 Defense[2];

	int32 Max_hp;

	int32 Hp;

	int32 Max_mp;

	int32 Mp;

	bool Debuff[2];

	float Paralysis_Acc;

	float Poison_Acc;

	int32 Exp;

	float Attack_range;

	float Attack_speed;

	float Skill_cooltime[5];
	float Skill_cooltimeAcc[5];

	AnimType AnimState;

	int PortraitNum;

	bool isDead;

	EquipItemType MyItem;
};


USTRUCT(BlueprintType)
struct FPP_ControllerInfo// : public FTableRowBase
{
	GENERATED_BODY()

public:
	int32 Money;

	int32 Consume_item_count[4];

	int32 Ultimate_acc;
};

USTRUCT(BlueprintType)
struct FPP_ConsumeItemInfo// : public FTableRowBase
{
	GENERATED_BODY()

public:
	ConsumeItemType Type;

	int32 Price;

	int32 Recover_amount;
};

UCLASS()
class ELFPALADINTEMPLATE_API UPP_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPP_GameInstance();

protected:
	UPROPERTY()
	FPP_PlayerInfo CharacterArray[3];
	UPROPERTY()
	TArray<class UPP_EquipItemData*> EquipItemDatas;
	UPROPERTY()
	TArray<EquipItemType> EquipItemArraytemp;
	UPROPERTY()
	TArray<FString> EquipItemOwner;
	UPROPERTY()
	bool isSave;
	UPROPERTY()
	int Questnum;
	UPROPERTY()
	int Gold;

public:
	void SetPlayerInfo(FPP_PlayerInfo* data, int index);
	void AddItem(class UPP_EquipItemData* data);
	void ItemBackUp();
	void ItemOwnerBackUp();
	void ItemRestore();
	void SetQuestNum(int num)
	{
		Questnum = num;
	}
	int GetQuestNum()
	{
		return Questnum;
	}

	bool IsSave()
	{ 
		return isSave; 
	}
	void SetSave(bool is)
	{ 
		isSave = is; 
	}
	FPP_PlayerInfo* GetPlayerInfo(int index)
	{
		return &CharacterArray[index];
	}
	TArray<class UPP_EquipItemData*> GetEquipItemData() 
	{
		return EquipItemDatas; 
	}
	int GoldUp(int amount)
	{
		Gold += amount;
		return Gold;
	}
	int GoldDown(int amount)
	{
		if ((Gold - amount) < 0)
			return -1;
		Gold -= amount;
		return Gold;
	}
};
