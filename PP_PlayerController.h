// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_GameInstance.h"
#include "PP_Player.h"
#include "GameFramework/PlayerController.h"
#include "PP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
		APP_PlayerController();
		virtual void PlayerTick(float DeltaTime) override;
		virtual void SetupInputComponent();
		void MovePicking();
		void MouseLClick();
		void MouseLClickRelease();
		void SetSingleGuard();
		void SetAutoBattleOnOff();
		void SetPartyGuard();
		void SetPartyAutoBattleOnOff();


		void SetControlCharacter(APP_Player* Char_p)
		{
			ControlCharacter = Char_p;
		}
		APP_Player* GetControlCharacter()
		{
			return ControlCharacter;
		}
		void SetSkillTarget(int skillnum = 0, bool isplayertarget = false)
		{
			if (ControlCharacter->GetPlayerInfo()->Debuff[0])
				return;
			isTargettingSkill = true;
			isPlayerTarget = isplayertarget;
			SkillNum = skillnum;
		}
		void SkillTargetFree()
		{
			isTargettingSkill = false;
			isPlayerTarget = false;
			SkillNum = 0;
		}
		void SetUsingItemOn(ConsumeItemType type)
		{
			isTargetItem = true;
			ItemType = type;
		}
		void SetUsingItemOff()
		{
			isTargetItem = false;
		}


protected:
	UPROPERTY()
	bool	LButtonClicked;
	UPROPERTY()
	APP_Player* ControlCharacter;
	UPROPERTY()
	TArray<APP_Player*> MyParty;
	UPROPERTY()
	class UPP_MainWidgetFrame* MyMainWidget;
	UPROPERTY()
	class UPP_Quset* Quest;
	UPROPERTY()
	class UPP_GameInstance* MyInstance;
	UPROPERTY()
	TArray<class UPP_EquipItemData*> MyEquipItems;
	UPROPERTY()
	class APostProcessVolume* Postprocess_phy;
	UPROPERTY()
	float goalacc;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;

	UPROPERTY()
	bool isTargettingSkill;
	UPROPERTY()
	bool isTargetItem;
	UPROPERTY()
	ConsumeItemType ItemType;
	UPROPERTY()
	bool isPartyGuard;
	UPROPERTY()
	bool isPartyAuttoBattle;
	UPROPERTY()
	bool isAssemble;
	UPROPERTY()
	float assemble_tick;
	UPROPERTY()
	float assemble_dealay;
	UPROPERTY()
	int SkillNum;
	UPROPERTY()
	bool isPlayerTarget;

public:
	void CameraTurn(float fScale);
	void CameraZoom(float fScale);
	void CameraLookUp(float fScale);

	class UPP_MainWidgetFrame* GetMainWidget();

	int32 PlayerInsert(APP_Player* player);
	bool AssembleOnOff()
	{
		if (!isPartyGuard && !isPartyAuttoBattle)//가드나 자동전투가 아닌경우에만 성공
		{
			isAssemble = !isAssemble;
			if (isAssemble == false)
			{
				for (auto& myCharcyer : MyParty)
				{
					if (myCharcyer != ControlCharacter)
						myCharcyer->SimpleStop();
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	TArray<APP_Player*>* GetMyParty()
	{
		return &MyParty;
	}

	//void SetMonsterHuntting(const FName& name);
	void SetMonsterHuntting(MonsterType name);
	class UPP_Quset* GetQuestBoard();

	void ChangeEquip();

	void ObtainItem(int num);
	void BuyItem(int num);

	bool isControllCharAlive();

	void ObtainExps(int amount);

	void CharacterSave(FName NextLevel, bool isLevelChange = false);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
