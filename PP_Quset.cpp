// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Quset.h"
#include "PP_QuestWidget.h"


UPP_Quset::UPP_Quset()
{
	QuestDesc.Reset();
	QuestName.Reset();
	TargetName = MonsterType::none;
	isBattle = true;
	GoalNum = 9999;
	MonsterCnt = 0;
	GoalPostion = FVector(0, 0, -99999);
	isClear = false;
	Distance = 9999;
}

void UPP_Quset::SetQuest(bool is_battle, int goal_num, FString quest_name, FString quest_desc, const FName& target_name, MonsterType monster_type, FVector goal_postion)
{
	//버그때문에 초기화
	QuestDesc.Reset();
	QuestName.Reset();
	TargetName = MonsterType::none;
	isBattle = true;
	GoalNum = 9999;
	MonsterCnt = 0;
	GoalPostion = FVector(0, 0, -99999);
	isClear = false;
	Distance = 9999;




	if(Quest_Board)
		Quest_Board->SetVisibility(ESlateVisibility::Visible);
	isBattle = is_battle;
	isClear = false;
	QuestDesc = quest_desc;
	QuestName = quest_name;
	if (isBattle) //전투퀘
	{
		GoalNum = goal_num;
		MonsterCnt = 0;
		TargetName = monster_type;
		if (Quest_Board)
		{
			//FText temptext = FText::AsNumber(GoalNum - MonsterCnt);
			//Quest_Board->SetTargetText(temptext);
			Quest_Board->SetTargetText(FString::Printf(TEXT("%d / %d"), MonsterCnt, GoalNum));
		}
	}
	else //이동퀘
	{
		GoalPostion.X = goal_postion.X;
		GoalPostion.Y = goal_postion.Y;
		GoalPostion.Z = goal_postion.Z;
		if (Quest_Board)
		{
			//FText temptext = FText::AsNumber(Distance);
			//Quest_Board->SetTargetText(temptext);
			Quest_Board->SetTargetText(FString::Printf(TEXT("Dist : %f"), Distance));
		}
	}

	if (Quest_Board)
	{
		FText temptext1 = FText::FromString(quest_desc);
		Quest_Board->SetGoalDescText(temptext1);

		FText temptext2 = FText::FromString(QuestName);
		Quest_Board->SetNameText(temptext2);
	}
}
/*
bool UPP_Quset::CheckMonster(const FName& monster)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TargetName.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, monster.ToString());
	if (isBattle && !isClear)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TargetName + FString::Printf(TEXT(" ===== ")) + monster);

		if (TargetName.IsEqual(monster))
		{
			MonsterCnt++;
			if (MonsterCnt >= GoalNum)
			{
				isClear = true;
				Quest_Board->SetVisibility(ESlateVisibility::Collapsed);
				set_Quest_step_by_step();
			}
			else
			{
				if (Quest_Board)
				{
					Quest_Board->SetTargetText(TargetName.ToString() +FString::Printf(TEXT(" : %d / %d"), MonsterCnt, GoalNum));
				}
			}
		}
	}
	return isClear;
}
*/
bool UPP_Quset::CheckMonster(MonsterType type)
{
	if (quest_num == 1 || quest_num == 3 || quest_num == 5 || quest_num == 7 || quest_num == 9 || quest_num == 11 || quest_num == 13)
	{
		if (isBattle && !isClear)
		{
			//3가지 변수타입 전부 다 비교가 안되서 그냥 번호에 따른 비교로..
			if ((quest_num == 1 && type == MonsterType::Goblin) || (quest_num == 3 && type == MonsterType::Orc) || (quest_num == 5 && type == MonsterType::Scorpi) || (quest_num == 7 && type == MonsterType::Arachne)
				|| (quest_num == 9 && type == MonsterType::TectacleMonster)
				|| (quest_num == 11 && type == MonsterType::Satan)
				|| (quest_num == 13 && type == MonsterType::Lavos))
			{
				MonsterCnt++;
					if (MonsterCnt >= GoalNum)
					{
						isClear = true;
							if (Quest_Board)
								Quest_Board->SetVisibility(ESlateVisibility::Collapsed);
						set_Quest_step_by_step();
					}
					else
					{
						if (Quest_Board)
						{
							//FText temptext = FText::AsNumber(GoalNum - MonsterCnt);
							//Quest_Board->SetTargetText(temptext);
							Quest_Board->SetTargetText(FString::Printf(TEXT("%d / %d"), MonsterCnt, GoalNum));
						}
					}
			}
		}
	}
	return isClear;
}


float UPP_Quset::CheckGoal(FVector pos)
{
	Distance = 9999;
	if (!isBattle && !isClear)
	{
		Distance = FVector::Dist(pos, GoalPostion);
		if (Distance <= 300)
		{
			isClear = true;
			if (Quest_Board)
				Quest_Board->SetVisibility(ESlateVisibility::Collapsed);
			set_Quest_step_by_step();
		}
		else
		{
			if (Quest_Board)
			{
				//FText temptext = FText::AsNumber(Distance);
				//Quest_Board->SetTargetText(temptext);
				Quest_Board->SetTargetText(FString::Printf(TEXT("Dist : %.3f"), Distance));
			}
		}
	}
	return Distance;
}

void UPP_Quset::SetQuestBoard(UPP_QuestWidget* board)
{
	Quest_Board = board;
}

//임시진행퀘스트
void UPP_Quset::set_Quest_step_by_step()
{
	quest_num++;
	temp[0].Reset();
	temp[1].Reset();
	switch (quest_num)
	{
	case 1:
	{
		temp[0] = FString(TEXT("Goblin"));
		temp[1] = FString(TEXT("The castle was attacked by the demon army. Kill 15 goblins first"));
		SetQuest(true, 15, temp[0], temp[1], FName("Goblin"), MonsterType::Goblin); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 2:
	{
		temp[0] = FString(TEXT("Gate"));
		temp[1] = FString(TEXT("Killed all the monsters in the castle. let's go to the gate"));
		SetQuest(false, 0, temp[0], temp[1], FName("Door"), MonsterType::none, FVector(-24083.224609, 15460.894531, 8391.061523)); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 3:
	{
		temp[0] = FString(TEXT("Orc"));
		temp[1] = FString(TEXT("In front of the gate, the gate of the demons was open and the Orc general was waiting.kill the orcs"));
		SetQuest(true, 3, temp[0], temp[1], FName("Orc"), MonsterType::Orc); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 4:
	{
		temp[0] = FString(TEXT("Portal"));
		temp[1] = FString(TEXT("Let's go to the demon's stronghold by taking the portal to counterattack."));
		SetQuest(false, 0, temp[0], temp[1], FName("Portal"), MonsterType::none, FVector(-31260.0, 17340.0, 7130.0)); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 5:
	{
		temp[0] = FString(TEXT("Scorpi"));
		temp[1] = FString(TEXT("Let's kill the monsters around the entrance"));
		SetQuest(true, 3, temp[0], temp[1], FName("Scorpi"), MonsterType::Scorpi); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 6:
	{
		temp[0] = FString(TEXT("Demon's Gate"));
		temp[1] = FString(TEXT("Let's go inside the demon's stronghold"));
		SetQuest(false, 0, temp[0], temp[1], FName("Gate"), MonsterType::none, FVector(-1809.916138, 6444.812012, -6632.498535)); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 7:
	{
		temp[0] = FString(TEXT("Arachne"));
		temp[1] = FString(TEXT("Let's kill the monster guarding the castle gate"));
		SetQuest(true, 1, temp[0], temp[1], FName("Arachne"), MonsterType::Arachne); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 8:
	{
		temp[0] = FString(TEXT("Stronghold"));
		temp[1] = FString(TEXT("Now that we've caught the Arachne, let's enter the demon's stronghold."));
		SetQuest(false, 0, temp[0], temp[1], FName("Dungeon"), MonsterType::none, FVector(-3770.0, 13140.0, -6830.0)); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 9:
	{
		temp[0] = FString(TEXT("Tectacles"));
		temp[1] = FString(TEXT("Let's kill the monsters inside the stronghold"));
		SetQuest(true, 5, temp[0], temp[1], FName("TectacleMonster"), MonsterType::TectacleMonster); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 10:
	{
		temp[0] = FString(TEXT("Demon's Room"));
		temp[1] = FString(TEXT("All the monsters inside the stronghold. Let's go to the final battle"));
		SetQuest(false, 0, temp[0], temp[1], FName("Dungeon"), MonsterType::none, FVector(3140.0, -3530.0, -480.0)); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 11:
	{
		temp[0] = FString(TEXT("Kill Satan"));
		temp[1] = FString(TEXT("Last Battle...Maybe"));
		SetQuest(true, 1, temp[0], temp[1], FName("Satan"), MonsterType::Satan); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 12:
	{
		temp[0] = FString(TEXT("Last Gate"));
		temp[1] = FString(TEXT("The gate at the back is suspicious. let's check"));
		SetQuest(false, 0, temp[0], temp[1], FName("Last"), MonsterType::none, FVector(3170.0, -7790.0, -310.0)); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 13:
	{
		temp[0] = FString(TEXT("Lavos"));
		temp[1] = FString(TEXT("We moved to another dimension, but baby Phoenix gave me a foothold. Defeat the Lavos for everyone's future!"));
		SetQuest(true, 1, temp[0], temp[1], FName("LaBoss"), MonsterType::Lavos); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	default:
		if (Quest_Board)
			Quest_Board->SetVisibility(ESlateVisibility::Collapsed);
	}
}