// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_PlayerController.h"
#include "PP_Player.h"
#include "PP_Monster.h"
#include "Kismet/GameplayStatics.h"
#include "PP_MainGameModeBase.h"
#include "PP_MainWidgetFrame.h"
#include "PP_Quset.h"
#include "PP_EquipWidgeet.h"

#include "PP_EquipItemData.h"

#include "Engine/PostProcessVolume.h"

#include "PP_ShopWidget.h"
#include "PP_ShopNPC.h"

APP_PlayerController::APP_PlayerController()
{
	LButtonClicked = false;
	//SetViewTargetWithBlend()

	//마우스 조작용 설정 활성화
	SetShowMouseCursor(true);
	bEnableClickEvents = true;

	goalacc = 0;
}

void APP_PlayerController::BeginPlay()
{
	FString test;

	//Super::UnPossess();
	SkillTargetFree();
	//PlayerCameraManager->bClientSimulatingViewTarget = PlayerCameraManager->bUseClientSideCameraUpdates = PlayerCameraManager->bGameCameraCutThisFrame =
		// 0;//PlayerCameraManager->bAlwaysApplyModifiers = 0;
	//PlayerCameraManager->FreeCamOffset = PlayerCameraManager->ViewTargetOffset = FVector::ZeroVector;
	isPartyGuard = isPartyAuttoBattle = false;

	//파티 집합명령
	assemble_dealay = .5f;
	assemble_tick = 0;
	isAssemble = true;
	isPlayerTarget = false;

	APP_MainGameModeBase* temp_gamemode = Cast<APP_MainGameModeBase>(GetWorld()->GetAuthGameMode());
	if (temp_gamemode)
	{
		MyMainWidget = Cast<UPP_MainWidgetFrame>(temp_gamemode->GetMainWidget());
		if (MyMainWidget)
		{
			;//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OK %f %f %f %f"), FMath::Sin(PI / 6), FMath::Sin(PI / 4), FMath::Sin(PI / 3), FMath::Sin(PI / 2)));
		}
	}

	
	MyInstance = Cast<UPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyInstance)
	{
		if (!MyInstance->IsSave())//처음맵
		{
			//기본 아이템 설정 임시
			/*
			UPP_EquipItemData* Data1 = NewObject<UPP_EquipItemData>(MyInstance,
				UPP_EquipItemData::StaticClass());
			Data1->SetMyItem(EquipItemType::Power);
			MyInstance->AddItem(Data1);
			
			UPP_EquipItemData* Data2 = NewObject<UPP_EquipItemData>(MyInstance,
				UPP_EquipItemData::StaticClass());
			Data2->SetMyItem(EquipItemType::Defense);
			MyInstance->AddItem(Data2);

			UPP_EquipItemData* Data3 = NewObject<UPP_EquipItemData>(MyInstance,
				UPP_EquipItemData::StaticClass());
			Data3->SetMyItem(EquipItemType::SoftPower);
			MyInstance->AddItem(Data3);

			UPP_EquipItemData* Data4 = NewObject<UPP_EquipItemData>(MyInstance,
				UPP_EquipItemData::StaticClass());
			Data4->SetMyItem(EquipItemType::SoftDefense);
			MyInstance->AddItem(Data4);

			UPP_EquipItemData* Data5 = NewObject<UPP_EquipItemData>(MyInstance,
				UPP_EquipItemData::StaticClass());
			Data5->SetMyItem(EquipItemType::Mix);
			MyInstance->AddItem(Data5);
			*/



			MyMainWidget->SetEquipItem(MyInstance->GetEquipItemData());
			//초기화
			MyMainWidget->GetEquipWidget()->SetImageAndVisible(0, 0, true);

			Quest = NewObject<UPP_Quset>(this);
			if (Quest)
			{
				Quest->SetQuestBoard(MyMainWidget->GetQuestBoard());
				Quest->quest_num = 0;
				Quest->set_Quest_step_by_step();
			}
		}
		else
		{
			MyInstance->ItemRestore();
			MyMainWidget->SetEquipItem(MyInstance->GetEquipItemData());
			//아이템 재착용
			for (int i = 0; i < MyInstance->GetEquipItemData().Num(); i++)
			{
				FString tempjob = (MyInstance->GetEquipItemData())[i]->GetOwner();
			
				for (auto& character : MyParty)
				{
					if (character->GetPlayerInfo()->Job == tempjob)
					{
			
						character->SetItem((MyInstance->GetEquipItemData())[i]->GetMyItem());
						
						break;
					}
				}
			}
			MyMainWidget->GetEquipWidget()->SetImageAndVisible(0, (int)(MyParty[0]->GetPlayerInfo()->MyItem), true);
			
			//MyMainWidget->GetEquipWidget()->SetImageAndVisible(0, 0, true);
			
			Quest = NewObject<UPP_Quset>(this);
			if (Quest)
			{
				Quest->SetQuestBoard(MyMainWidget->GetQuestBoard());
				Quest->quest_num = (MyInstance->GetQuestNum() - 1);
				Quest->set_Quest_step_by_step();
			}
		}
	}
	
	if (MyMainWidget)
	{
		MyMainWidget->GetShopWidget()->OpenShop();
		MyMainWidget->ShopClose();
	}
	

	//마비용 포스트프로세스 찾기
	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "post_pha", temp);
	if(temp[0])
		Postprocess_phy = Cast<APostProcessVolume>(temp[0]);
}

void APP_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!ControlCharacter) //첫 컨트롤 캐릭터 설정
	{
		if (MyParty.Num() >= 3)
		{
			for (auto& character : MyParty)
			{
				if (character->GetPlayerInfo()->PortraitNum == 1)
				{
					ControlCharacter = character;
					break;
				}
			}
		}
		ChangeEquip();
	}
	//마비상태의 포스트프로세스설정
	if (ControlCharacter && Postprocess_phy)
	{
		if (ControlCharacter->GetPlayerInfo()->Debuff[0])
		{
			Postprocess_phy->bEnabled = true;
		}
		else
		{
			Postprocess_phy->bEnabled = false;
		}
	}
	else if (Postprocess_phy)
	{
		Postprocess_phy->bEnabled = false;
	}
	else
	{
		;
	}

	if (Quest->IsMoveQuest() && ControlCharacter && !Quest->IsClear()) //이동퀘중에 거리 실시간 변경
	{
		goalacc += DeltaTime;
		if (goalacc >= 0.03f)
		{
			goalacc = 0;
			Quest->CheckGoal(ControlCharacter->GetActorLocation());
		}
	}

	if (LButtonClicked)
	{
		MovePicking();
		LButtonClicked = false;
	}
	if (ControlCharacter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f %f %f %f"), FMath::Sin(PI/6), FMath::Sin(PI / 4), FMath::Sin(PI / 3), FMath::Sin(PI / 2)));
		assemble_tick += DeltaTime;
		if (assemble_dealay <= assemble_tick)
		{
			assemble_tick = 0;
			int cnt = 1;
			FVector foward = ControlCharacter->GetMeshForwardVector();
			for (auto& myCharcyer : MyParty)
			{
				if (myCharcyer->GetPlayerInfo()->Debuff[0] == true || myCharcyer->GetPlayerInfo()->isDead) //마비거나 죽었으면 움직이지 않음
					return;
				myCharcyer->SetAssemble(isAssemble);
				if (myCharcyer == ControlCharacter)
				{
					continue;
				}
				else
				{
					if (isAssemble && !isPartyGuard && !isPartyAuttoBattle)//자동전투거나 가드중에는 불가능
					{
						FVector upfoward = foward * 250;
						float x = FMath::Cos(PI * cnt * 2 / 3) * upfoward.X - FMath::Sin(PI * cnt * 2 / 3) * upfoward.Y;
						float y = FMath::Sin(PI * cnt * 2 / 3) * upfoward.X + FMath::Cos(PI * cnt * 2 / 3) * upfoward.Y;
						FVector mypos = ControlCharacter->GetActorLocation() + FVector(x, y, 0);
						float distx = FMath::Abs(mypos.X - myCharcyer->GetActorLocation().X);
						float disty = FMath::Abs(mypos.Y - myCharcyer->GetActorLocation().Y);
						if(distx + disty >= 80)
							myCharcyer->SimpleMove(mypos);
						cnt++;
					}
				}
			}
		}
	}
}

void APP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("MouseL"),
		EInputEvent::IE_Pressed, this, &APP_PlayerController::MouseLClick);
	InputComponent->BindAction(TEXT("MouseL"),
		EInputEvent::IE_Released, this, &APP_PlayerController::MouseLClickRelease);
	InputComponent->BindAction(TEXT("Guard"),
		EInputEvent::IE_Pressed, this, &APP_PlayerController::SetSingleGuard);

	InputComponent->BindAxis(TEXT("CameraTurn"), this, &APP_PlayerController::CameraTurn);
	InputComponent->BindAxis(TEXT("CameraZoom"), this, &APP_PlayerController::CameraZoom);
	InputComponent->BindAxis(TEXT("CameraLookUp"), this, &APP_PlayerController::CameraLookUp);
}

//마우스 클릭으로 캐릭터 이동
void APP_PlayerController::MovePicking()
{
	FHitResult	result;

	if (ControlCharacter)
	{
		if (ControlCharacter->GetPlayerInfo()->Debuff[0])//마비상태면 조작 불가능
			return;
	}

	//bool bHit = GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic,
	//	false, result);

	bool bHit = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,
			false, result);

	if (bHit)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), result.Location.X, result.Location.Y, result.Location.Z));
		if (ControlCharacter && !ControlCharacter->GetPlayerInfo()->Debuff[0])
		{
			APP_Player* isPlayer = Cast<APP_Player>(result.Actor);
			APP_Monster* isMon = Cast<APP_Monster>(result.Actor);
			if (isMon && !ControlCharacter->GetPlayerInfo()->isDead) //몬스터클릭
			{
				AActor* temptarget = Cast<AActor>(result.Actor);
				if (temptarget)
				{
					if (isTargettingSkill) //타겟스킬 대기중이면
					{
						ControlCharacter->SetSKillTarget(temptarget, SkillNum);
						ControlCharacter->TargetFree();
						//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill target on")));
					}
					else
						ControlCharacter->SetTargetMonster(temptarget);
				}
			}
			else if (isPlayer) //플레이어클릭
			{ 
				if (isTargetItem  && !ControlCharacter->GetPlayerInfo()->isDead && !ControlCharacter->GetPlayerInfo()->Debuff[0]) //소비템 사용대기중이면
				{
					ControlCharacter->UsingItem(ItemType, isPlayer);
					SetUsingItemOff();
				}
				else if (isTargettingSkill && isPlayerTarget && !ControlCharacter->GetPlayerInfo()->isDead && !ControlCharacter->GetPlayerInfo()->Debuff[0])
				{
					ControlCharacter->SetSKillTarget(isPlayer, SkillNum);
					ControlCharacter->TargetFree();
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill target on")));
				}
				else //컨트롤캐릭전환
				{
				
					ControlCharacter = isPlayer;
					ControlCharacter->SetAutoBattleOnOff(false, false);
					SetViewTargetWithBlend(Cast<AActor>(ControlCharacter), 0.7f, EViewTargetBlendFunction::VTBlend_EaseIn, 15, true);
					//스킬아이콘 전환
					if (MyMainWidget)
						MyMainWidget->ChangeSkill_Icon(ControlCharacter->GetPlayerInfo()->PortraitNum);
					//장비창 전환
					ChangeEquip();
					//MyMainWidget->GetEquipWidget()->SetImageAndVisible(ControlCharacter->GetPlayerInfo()->PortraitNum - 1, (int)(ControlCharacter->GetPlayerInfo()->MyItem), true);
				}
			}
			else
			{
				APP_ShopNPC* isShop = Cast<APP_ShopNPC>(result.Actor);
				if(isShop)//상점클릭
				{
					float dist = FVector::Distance(isShop->GetActorLocation(), ControlCharacter->GetActorLocation());
					if(dist < 800)
					{
						isShop->SetTrade(true);
						MyMainWidget->ShopOpen();
					}
				}

				if(!ControlCharacter->GetSkillTarget() && !ControlCharacter->GetPlayerInfo()->isDead
					&& !ControlCharacter->GetPlayerInfo()->Debuff[0] && !ControlCharacter->IsSkill())//스킬중이면 이동불가
					ControlCharacter->SimpleMove(FVector(result.Location.X, result.Location.Y, result.Location.Z));
			}
		}
		else
		{
			//임시로 캐릭터 찾기
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "PC", Actors);
			if (Actors[0])
			{
				ControlCharacter = Cast<APP_Player>(Actors[0]);
				if (ControlCharacter)
				{
					ControlCharacter->SimpleMove(FVector(result.Location.X, result.Location.Y, result.Location.Z));
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("actor set error")));
				}
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("tag find error")));
			}
		}
	}
	//다른거 눌렀을 시 초기화
	SkillTargetFree();
	SetUsingItemOff();
}

void APP_PlayerController::MouseLClick()
{
	LButtonClicked = true;
}

void APP_PlayerController::MouseLClickRelease()
{
	LButtonClicked = false;
}

void APP_PlayerController::SetSingleGuard()
{
	if (ControlCharacter && !ControlCharacter->GetPlayerInfo()->isDead)
	{
		if(!ControlCharacter->GetPlayerInfo()->Debuff[0])//마비가 아니면
			ControlCharacter->Guard();
	}
}

void APP_PlayerController::SetAutoBattleOnOff()
{
	if (ControlCharacter && !ControlCharacter->GetPlayerInfo()->isDead)
	{
		if (!ControlCharacter->GetPlayerInfo()->Debuff[0])//마비가 아니면
			ControlCharacter->SetAutoBattleOnOff();
	}
}

void APP_PlayerController::SetPartyGuard()
{
	if (ControlCharacter && !ControlCharacter->GetPlayerInfo()->isDead)
	{
		isPartyGuard = !isPartyGuard;
		for (auto& character : MyParty)
		{
			if (!character->GetPlayerInfo()->Debuff[0])//마비가 아니면
				character->Guard(true, isPartyGuard);
		}
	}
}

void APP_PlayerController::SetPartyAutoBattleOnOff()
{
	if (ControlCharacter && !ControlCharacter->GetPlayerInfo()->isDead)
	{
		isPartyAuttoBattle = !isPartyAuttoBattle;
		for (auto& character : MyParty)
		{
			character->SetAutoBattleOnOff(true, isPartyAuttoBattle);
		}
	}
}

void APP_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("pc ride on the character")));
}

void APP_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("pc unride on the character")));
}



void APP_PlayerController::CameraTurn(float fScale)
{
	if (ControlCharacter)
		ControlCharacter->CameraTurn(fScale);
}
void APP_PlayerController::CameraZoom(float fScale)
{
	if (ControlCharacter)
		ControlCharacter->CameraZoom(fScale);
}
void APP_PlayerController::CameraLookUp(float fScale)
{
	if (ControlCharacter)
		ControlCharacter->CameraLookUp(fScale);
}

void APP_PlayerController::SetMonsterHuntting(MonsterType name)
{
	if (Quest)
	{
		Quest->SetQuestBoard(MyMainWidget->GetQuestBoard()); //버그나서 중복코드..
		Quest->CheckMonster(name);
	}
}

UPP_Quset* APP_PlayerController::GetQuestBoard()
{
	return Quest;
}

void APP_PlayerController::ChangeEquip()
{
	if(ControlCharacter)
		MyMainWidget->GetEquipWidget()->SetImageAndVisible(ControlCharacter->GetPlayerInfo()->PortraitNum - 1, (int)(ControlCharacter->GetPlayerInfo()->MyItem), true);
}

void APP_PlayerController::ObtainItem(int num)
{
	switch (num)
	{
	case 1:
	{
		UPP_EquipItemData* Data1 = NewObject<UPP_EquipItemData>(MyInstance,
			UPP_EquipItemData::StaticClass());
		Data1->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_1.EquipItem_1'"));
		Data1->SetString(FString("attack + 2000"));
		Data1->SetOwner(FString(""));
		Data1->SetMyItem(EquipItemType::Power);
		MyInstance->AddItem(Data1);
	}
	break;
	case 2:
	{
		UPP_EquipItemData* Data2 = NewObject<UPP_EquipItemData>(MyInstance,
			UPP_EquipItemData::StaticClass());
		Data2->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_2.EquipItem_2'"));
		Data2->SetString(FString("defense + 2000"));
		Data2->SetOwner(FString(""));
		Data2->SetMyItem(EquipItemType::Defense);
		MyInstance->AddItem(Data2);
	}
	break;
	case 3:
	{
		UPP_EquipItemData* Data2 = NewObject<UPP_EquipItemData>(MyInstance,
			UPP_EquipItemData::StaticClass());
		Data2->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_3.EquipItem_3'"));
		Data2->SetString(FString("attack + 1000"));
		Data2->SetOwner(FString(""));
		Data2->SetMyItem(EquipItemType::SoftPower);
		MyInstance->AddItem(Data2);
	}
	break;
	case 4:
	{
		UPP_EquipItemData* Data2 = NewObject<UPP_EquipItemData>(MyInstance,
			UPP_EquipItemData::StaticClass());
		Data2->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_4.EquipItem_4'"));
		Data2->SetString(FString("defense + 1000"));
		Data2->SetOwner(FString(""));
		Data2->SetMyItem(EquipItemType::SoftDefense);
		MyInstance->AddItem(Data2);
	}
	break;
	case 5:
	{
		UPP_EquipItemData* Data2 = NewObject<UPP_EquipItemData>(MyInstance,
			UPP_EquipItemData::StaticClass());
		Data2->SetIconTexture(TEXT("Texture2D'/Game/0_PP/PP_UI/Equip/EquipItem_5.EquipItem_5'"));
		Data2->SetString(FString("Attack and Defense + 1500"));
		Data2->SetOwner(FString(""));
		Data2->SetMyItem(EquipItemType::Mix);
		MyInstance->AddItem(Data2);
	}
	break;
	default :
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Item index error")));
		return;
	}
	}

	MyMainWidget->SetEquipItem(MyInstance->GetEquipItemData());
}

void APP_PlayerController::BuyItem(int num)
{
	ObtainItem(num);
	MyMainWidget->SetEquipItem(MyInstance->GetEquipItemData());
	MyMainWidget->GetShopWidget()->SetGold();
}

bool APP_PlayerController::isControllCharAlive()
{
	if (ControlCharacter->GetPlayerInfo()->Hp <= 0)
		return false;
	else
		return true;
}

void APP_PlayerController::ObtainExps(int amount)
{
	for (auto& character : MyParty)
	{
		if (!character->GetPlayerInfo()->isDead)//죽으면 경치안줌
			character->ObtainExp(amount);
	}
}

void APP_PlayerController::CharacterSave(FName NextLevel, bool isLevelChange)
{
	UPP_GameInstance* tempInstance = Cast<UPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (tempInstance)
	{
		for (auto& character : MyParty)
		{
			character->RemoveEquipItem();
			tempInstance->SetPlayerInfo(character->GetPlayerInfo(), character->GetPlayerInfo()->PortraitNum - 1);
		}
		tempInstance->SetSave(true);
		tempInstance->ItemBackUp();
		tempInstance->ItemOwnerBackUp();
		tempInstance->SetQuestNum(Quest->quest_num);
	}
	if (isLevelChange)
	{
		UGameplayStatics::OpenLevel(GetWorld(), NextLevel);
	}
}

void APP_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//CharacterSave();
}

int32 APP_PlayerController::PlayerInsert(APP_Player* player)
{
	MyParty.Add(player);
	if (MyParty.Num() == 3)
	{
		//세이브 불러오기
		MyInstance = Cast<UPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (MyInstance)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, FString::Printf(TEXT("Item Save Instance Load OK")));
			if (MyInstance->IsSave())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, FString::Printf(TEXT("Is Save OK")));
				//아이템 재착용
				for (int i = 0; i < MyInstance->GetEquipItemData().Num(); i++)
				{

					FString tempjob = (MyInstance->GetEquipItemData())[i]->GetOwner();

					//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, FString::Printf(TEXT("Item Num %d"),i) + tempjob);
					for (auto& character : MyParty)
					{
						if (character->GetPlayerInfo()->Job.Equals(tempjob))
						{
							character->SetItem((MyInstance->GetEquipItemData())[i]->GetMyItem());
							//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Blue, FString::Printf(TEXT("Item set ok")));
							break;
						}
					}
				}
				MyMainWidget->GetEquipWidget()->SetImageAndVisible(0, (int)(MyParty[0]->GetPlayerInfo()->MyItem), true);
			}
		}
		/*
		if(MyMainWidget)
			MyMainWidget->GetEquipWidget()->SetImageAndVisible(0, (int)(MyParty[0]->GetPlayerInfo()->MyItem), true);
		else
		{
			APP_MainGameModeBase* temp_gamemode = Cast<APP_MainGameModeBase>(GetWorld()->GetAuthGameMode());
			if (temp_gamemode)
				MyMainWidget = Cast<UPP_MainWidgetFrame>(temp_gamemode->GetMainWidget());
		}

		if (MyMainWidget)
			MyMainWidget->GetEquipWidget()->SetImageAndVisible(0, (int)(MyParty[0]->GetPlayerInfo()->MyItem), true);
			*/
	}
	return  MyParty.Num();
}

class UPP_MainWidgetFrame* APP_PlayerController::GetMainWidget()
{
	return MyMainWidget;
}