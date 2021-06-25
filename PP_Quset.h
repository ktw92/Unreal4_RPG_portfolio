// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "PP_GameInstance.h"
#include "UObject/NoExportTypes.h"
#include "PP_Quset.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_Quset : public UObject
{
	GENERATED_BODY()
public:
	UPP_Quset();

protected:
	UPROPERTY()
	FString QuestName;
	UPROPERTY()
	MonsterType TargetName;
	UPROPERTY()
	FString QuestDesc;
	UPROPERTY()
	bool isBattle;
	UPROPERTY()
	int GoalNum;
	UPROPERTY()
	int MonsterCnt;
	UPROPERTY()
	FVector GoalPostion;
	UPROPERTY()
	bool isClear;
	UPROPERTY()
	float Distance;

	UPROPERTY()
	class UPP_QuestWidget* Quest_Board;
	//이상하게 생성자로 하면 꺠져서 임시용
	UPROPERTY()
	FString temp[2];

public:
	void SetQuest(bool is_battle, int goal_num, FString quest_name, FString quest_desc, const FName& target_name, MonsterType monster_type = MonsterType::none, FVector goal_postion = FVector::ZeroVector);
	//bool CheckMonster(const FName& monster);
	bool CheckMonster(MonsterType type);
	float CheckGoal(FVector pos);
	void SetQuestBoard(UPP_QuestWidget* board);

	bool IsMoveQuest() { return !isBattle; }
	bool IsClear() { return isClear; }



	//임시용 변수
public:
	UPROPERTY()
	int quest_num;
	void set_Quest_step_by_step();
};
