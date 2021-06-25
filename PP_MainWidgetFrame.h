// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "PP_MainWidgetFrame.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class JobType : uint8
{
	SwordMaster,
	Witch,
	Pristess
};

UCLASS()
class ELFPALADINTEMPLATE_API UPP_MainWidgetFrame : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* AssembleBt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* AutoBattleSingleBT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* GuardSingleBT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* AutoBattlePartyBT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* GuardPartyBT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_1BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_2BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_3BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_4BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_5BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_1BT_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_2BT_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_3BT_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_4BT_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_5BT_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_1BT_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_2BT_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_3BT_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_4BT_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Skill_5BT_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Item_1BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Item_2BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Item_3BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* Item_4BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPP_PlayerPortraitWidget* PortraitWidget_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPP_PlayerPortraitWidget* PortraitWidget_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPP_PlayerPortraitWidget* PortraitWidget_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPP_MonsterStatusWidget* MonsterStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPP_QuestWidget* Quest_Board;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPP_MiniMapWidget* MiniMapWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPP_EquipWidgeet* EquipWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPP_InventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPP_ShopWidget* ShopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class UProgressBar*> SkillCool;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class UTexture2D*> KnightSkillIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class UTexture2D*> PristessSkillIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class UTexture2D*> WitchSkillIcons;

	/*
	오브젝트 파인더가 시용불가
	TSubclassOf<class UPP_DamageWidget> DamageWidget;
	*/

protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UWorld* MyWorld;
	UPROPERTY()
	JobType CharacterJob;

public:
	void SetWorld(UWorld* p) { MyWorld = p; };
	UFUNCTION()
	void Assemble();
	UFUNCTION()
	void AutoBattleSingle();
	UFUNCTION()
	void AutoBattleParty();
	UFUNCTION()
	void GuardSingle();
	UFUNCTION()
	void GuardParty();
	UFUNCTION()
	void Skill1();
	UFUNCTION()
	void Skill2();
	UFUNCTION()
	void Skill3();
	UFUNCTION()
	void Skill4();
	UFUNCTION()
	void Skill5();
	UFUNCTION()
	void Skill1_2();
	UFUNCTION()
	void Skill2_2();
	UFUNCTION()
	void Skill3_2();
	UFUNCTION()
	void Skill4_2();
	UFUNCTION()
	void Skill5_2();
	UFUNCTION()
		void Skill1_3();
	UFUNCTION()
		void Skill2_3();
	UFUNCTION()
		void Skill3_3();
	UFUNCTION()
		void Skill4_3();
	UFUNCTION()
		void Skill5_3();
	UFUNCTION()
	void Item1();
	UFUNCTION()
	void Item2();
	UFUNCTION()
	void Item3();
	UFUNCTION()
	void Item4();


	UPP_PlayerPortraitWidget* GetPortraitWidget(int num);

	UTexture2D* LoadTextureFromPath(const FString& Path);

	UPP_QuestWidget* GetQuestBoard();

	UPP_EquipWidgeet* GetEquipWidget();

	void ChangeSkill_Icon(int num = 1);
	class UPP_MiniMapWidget* GetMiniMap();

	void SetEquipItem(TArray<class UPP_EquipItemData*> Items);
	void SetEquipItem(UPP_EquipItemData* Item);
	void ShopOpen();
	void ShopClose();

	class UPP_ShopWidget* GetShopWidget();

	void SetSkillCoolTime(int index, float per);

	void ReSetSkillCollTime(int index);

};