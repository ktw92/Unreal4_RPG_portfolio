// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_MainWidgetFrame.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PP_PlayerController.h"
#include "PP_ElfSwordMaster.h"
#include "PP_HighPriestess.h"
#include "PP_Wizard.h"
#include "PP_PlayerPortraitWidget.h"
#include "PP_MonsterStatusWidget.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RenderUtils.h"

#include "PP_DamageWidget.h"

#include "PP_QuestWidget.h"

#include "PP_MiniMapWidget.h"

#include "PP_EquipWidgeet.h"

#include "PP_InventoryWidget.h"

#include "PP_ShopWidget.h"
#include "PP_ShopItemData.h"

void UPP_MainWidgetFrame::NativePreConstruct()
{
	Super::NativePreConstruct();

	/*
	static ConstructorHelpers::FClassFinder<UPP_DamageWidget>	asset(TEXT("Blueprint'/Game/0_PP/0_Monsters/ArachBoss/NewBlueprint1.NewBlueprint1_C'"));
	if (asset.Succeeded())
		DamageWidget = asset.Class;
	else
		////GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("nonesjdfjlarlhgoaerhto gthearog")));
		*/

	//임시
	CharacterJob = JobType::SwordMaster;

	AssembleBt = Cast<UButton>(GetWidgetFromName(TEXT("Button_Assemble")));
	AssembleBt->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Assemble);

	AutoBattleSingleBT = Cast<UButton>(GetWidgetFromName(TEXT("Button_AutoBattleSingle")));
	AutoBattleSingleBT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::AutoBattleSingle);

	GuardSingleBT = Cast<UButton>(GetWidgetFromName(TEXT("Button_GuardSingle")));
	GuardSingleBT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::GuardSingle);
	
	AutoBattlePartyBT = Cast<UButton>(GetWidgetFromName(TEXT("Button_AutoBattleParty")));
	AutoBattlePartyBT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::AutoBattleParty);

	GuardPartyBT = Cast<UButton>(GetWidgetFromName(TEXT("Button_GuardParty")));
	GuardPartyBT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::GuardParty);
	
	Skill_1BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_1")));
	Skill_1BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill1);

	Skill_2BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_2")));
	Skill_2BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill2);

	Skill_3BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_3")));
	Skill_3BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill3);

	Skill_4BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_4")));
	Skill_4BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill4);

	Skill_5BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_5")));
	Skill_5BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill5);

	Skill_1BT_2 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_1_2")));
	Skill_1BT_2->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill1_2);

	Skill_2BT_2 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_2_2")));
	Skill_2BT_2->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill2_2);

	Skill_3BT_2 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_3_2")));
	Skill_3BT_2->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill3_2);

	Skill_4BT_2 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_4_2")));
	Skill_4BT_2->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill4_2);

	Skill_5BT_2 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_5_2")));
	Skill_5BT_2->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill5_2);

	Skill_1BT_3 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_1_3")));
	Skill_1BT_3->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill1_3);

	Skill_2BT_3 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_2_3")));
	Skill_2BT_3->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill2_3);

	Skill_3BT_3 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_3_3")));
	Skill_3BT_3->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill3_3);

	Skill_4BT_3 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_4_3")));
	Skill_4BT_3->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill4_3);

	Skill_5BT_3 = Cast<UButton>(GetWidgetFromName(TEXT("Button_Skill_5_3")));
	Skill_5BT_3->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Skill5_3);

	Item_1BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Item_1")));
	Item_1BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Item1);

	Item_2BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Item_2")));
	Item_2BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Item2);

	Item_3BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Item_3")));
	Item_3BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Item3);

	Item_4BT = Cast<UButton>(GetWidgetFromName(TEXT("Button_Item_4")));
	Item_4BT->OnClicked.AddDynamic(this, &UPP_MainWidgetFrame::Item4);

	PortraitWidget_1 = Cast<UPP_PlayerPortraitWidget>(GetWidgetFromName(TEXT("Portrait_1")));
	PortraitWidget_2 = Cast<UPP_PlayerPortraitWidget>(GetWidgetFromName(TEXT("Portrait_2")));
	if (PortraitWidget_2)
		PortraitWidget_2->SetNameText("Pristess");
	PortraitWidget_3 = Cast<UPP_PlayerPortraitWidget>(GetWidgetFromName(TEXT("Portrait_3")));
	if (PortraitWidget_3)
		PortraitWidget_3->SetNameText("Wizard");

	MonsterStatus = Cast<UPP_MonsterStatusWidget>(GetWidgetFromName(TEXT("MonsterStatus")));

	Quest_Board = Cast<UPP_QuestWidget>(GetWidgetFromName(TEXT("QuestBoard")));

	MiniMapWidget = Cast<UPP_MiniMapWidget>(GetWidgetFromName(TEXT("MiniMap")));
	
	EquipWidget = Cast<UPP_EquipWidgeet>(GetWidgetFromName(TEXT("PP_BPEquipWidget")));

	InventoryWidget = Cast<UPP_InventoryWidget>(GetWidgetFromName(TEXT("PP_BPInventoryWidget")));

	ShopWidget = Cast<UPP_ShopWidget>(GetWidgetFromName(TEXT("PP_BPShopWidget")));

	SkillCool.Add(Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill1_ProgressBar"))));
	SkillCool.Add(Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill2_ProgressBar"))));
	SkillCool.Add(Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill3_ProgressBar"))));
	SkillCool.Add(Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill4_ProgressBar"))));
	SkillCool.Add(Cast<UProgressBar>(GetWidgetFromName(TEXT("Skill5_ProgressBar"))));
	for (int i = 0; i < SkillCool.Num(); i++)
	{
		ReSetSkillCollTime(i);
	}

}

void UPP_MainWidgetFrame::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPP_MainWidgetFrame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UPP_MainWidgetFrame::AutoBattleSingle()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("SingleAutoBattle BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp)
		{
			temp->SetAutoBattleOnOff();
		}
	}
}

void UPP_MainWidgetFrame::AutoBattleParty()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("ParyAutoBattle BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp)
		{
			temp->SetPartyAutoBattleOnOff();
		}
	}
}

void UPP_MainWidgetFrame::GuardSingle()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("SingleGuard BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp)
		{
			temp->SetSingleGuard();
		}
	}
}

void UPP_MainWidgetFrame::GuardParty()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("PartyGuard BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp)
		{
			temp->SetPartyGuard();
		}
	}
}

void UPP_MainWidgetFrame::Skill1()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Sk1 BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
		{
				switch (CharacterJob)
				{
				case JobType::SwordMaster:
				{
					APP_ElfSwordMaster* tempp = Cast<APP_ElfSwordMaster>(temp->GetControlCharacter());
					if (tempp)
					{
						if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
							return;
						tempp->ChangeSkill1();
					}
				}
					break;
				case JobType::Witch:;
					break;
				case JobType::Pristess:;
					break;
				default:;
					break;
				}
		}
	}
}
void UPP_MainWidgetFrame::Skill2()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Sk2 BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
		{
			switch (CharacterJob)
			{
			case JobType::SwordMaster:
			{
				APP_ElfSwordMaster* tempp = Cast<APP_ElfSwordMaster>(temp->GetControlCharacter());
				if (tempp)
				{
					if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
						return;
					tempp->ChangeSkill2();
				}
			}
			break;
			case JobType::Witch:;
				break;
			case JobType::Pristess:;
				break;
			default:;
				break;
			}
		}
	}
}
void UPP_MainWidgetFrame::Skill3()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Sk3 BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
		{
			switch (CharacterJob)
			{
			case JobType::SwordMaster:
			{
				APP_ElfSwordMaster* tempp = Cast<APP_ElfSwordMaster>(temp->GetControlCharacter());
				if (tempp)
				{
					if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
						return;
					tempp->ChangeSkill3();
				}
			}
			break;
			case JobType::Witch:;
				break;
			case JobType::Pristess:;
				break;
			default:;
				break;
			}
		}
	}
}
void UPP_MainWidgetFrame::Skill4()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Sk4 BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
		{
			switch (CharacterJob)
			{
			case JobType::SwordMaster:
			{
				temp->SetSkillTarget();
			}
			break;
			case JobType::Witch:;
				break;
			case JobType::Pristess:;
				break;
			default:;
				break;
			}
		}
	}
}
void UPP_MainWidgetFrame::Skill5()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Sk5 BT Click")));
	if (MyWorld)
	{
		APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
		if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
		{
			switch (CharacterJob)
			{
			case JobType::SwordMaster:
			{
				APP_ElfSwordMaster* tempp = Cast<APP_ElfSwordMaster>(temp->GetControlCharacter());
				if (tempp)
				{
					if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
						return;
					tempp->ChangeSkill5();
				}
			}
			break;
			case JobType::Witch:;
				break;
			case JobType::Pristess:;
				break;
			default:;
				break;
			}
		}
	}
}

void UPP_MainWidgetFrame::Item1()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Item1 BT Click")));
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetUsingItemOn(ConsumeItemType::Hp_potion);
	}
}

void UPP_MainWidgetFrame::Item2()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Item2 BT Click")));
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetUsingItemOn(ConsumeItemType::Mp_potion);
	}
}

void UPP_MainWidgetFrame::Item3()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Item3 BT Click")));
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetUsingItemOn(ConsumeItemType::Debuff_recover);
	}
}

void UPP_MainWidgetFrame::Item4()
{
	////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Item4 BT Click")));
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetUsingItemOn(ConsumeItemType::Resurrectioner);
	}
}

void UPP_MainWidgetFrame::Assemble()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp)
	{
		bool isok = temp->AssembleOnOff();
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("assemble bt ok = %d"), isok));
	}
}

UPP_PlayerPortraitWidget* UPP_MainWidgetFrame::GetPortraitWidget(int num)
{
	switch (num)
	{
	case 1: return PortraitWidget_1; ////GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Portrait 1 setting")));
		break;
	case 2: return PortraitWidget_2; ////GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Portrait 2 setting")));
		break;
	case 3: return PortraitWidget_3; ////GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Portrait 3 setting")));
		break;
	}
	return nullptr;
}

UTexture2D* UPP_MainWidgetFrame::LoadTextureFromPath(const FString& Path)
{
	UTexture2D* sprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));
	
	return sprite;
}

void UPP_MainWidgetFrame::ChangeSkill_Icon(int num)
{
	//귀찮아서 무식한 코드 수정 필요....버튼 배열로할걸..
	//일단 다 안보이게
	Skill_1BT->SetVisibility(ESlateVisibility::Collapsed);
	Skill_2BT->SetVisibility(ESlateVisibility::Collapsed);
	Skill_3BT->SetVisibility(ESlateVisibility::Collapsed);
	Skill_4BT->SetVisibility(ESlateVisibility::Collapsed);
	Skill_5BT->SetVisibility(ESlateVisibility::Collapsed);
	Skill_1BT_2->SetVisibility(ESlateVisibility::Collapsed);
	Skill_2BT_2->SetVisibility(ESlateVisibility::Collapsed);
	Skill_3BT_2->SetVisibility(ESlateVisibility::Collapsed);
	Skill_4BT_2->SetVisibility(ESlateVisibility::Collapsed);
	Skill_5BT_2->SetVisibility(ESlateVisibility::Collapsed);
	Skill_1BT_3->SetVisibility(ESlateVisibility::Collapsed);
	Skill_2BT_3->SetVisibility(ESlateVisibility::Collapsed);
	Skill_3BT_3->SetVisibility(ESlateVisibility::Collapsed);
	Skill_4BT_3->SetVisibility(ESlateVisibility::Collapsed);
	Skill_5BT_3->SetVisibility(ESlateVisibility::Collapsed);
	
	//선택한거 보여지게
	if (num == 3)
	{
		////GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Select char 3")));
		Skill_1BT_3->SetVisibility(ESlateVisibility::Visible);
		Skill_2BT_3->SetVisibility(ESlateVisibility::Visible);
		Skill_3BT_3->SetVisibility(ESlateVisibility::Visible);
		Skill_4BT_3->SetVisibility(ESlateVisibility::Visible);
		Skill_5BT_3->SetVisibility(ESlateVisibility::Visible);
	}
	else if (num == 2)
	{
		////GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Select char 2")));
		Skill_1BT_2->SetVisibility(ESlateVisibility::Visible);
		Skill_2BT_2->SetVisibility(ESlateVisibility::Visible);
		Skill_3BT_2->SetVisibility(ESlateVisibility::Visible);
		Skill_4BT_2->SetVisibility(ESlateVisibility::Visible);
		Skill_5BT_2->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		////GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Select char 1")));
		Skill_1BT->SetVisibility(ESlateVisibility::Visible);
		Skill_2BT->SetVisibility(ESlateVisibility::Visible);
		Skill_3BT->SetVisibility(ESlateVisibility::Visible);
		Skill_4BT->SetVisibility(ESlateVisibility::Visible);
		Skill_5BT->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPP_MainWidgetFrame::Skill1_2()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetSkillTarget(1, true);
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Pristess Skill1 on!")));
	}
	
}

void UPP_MainWidgetFrame::Skill2_2()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		APP_HighPriestess* tempp = Cast<APP_HighPriestess>(temp->GetControlCharacter());
		if (tempp)
		{
			if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
				return;
			tempp->Skill2();
			////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Pristess Skill2 on!")));
		}
	}
}

void UPP_MainWidgetFrame::Skill3_2()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetSkillTarget(3, true);
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Pristess Skill3 on!")));
	}
	
}

void UPP_MainWidgetFrame::Skill4_2()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetSkillTarget(4, true);
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Pristess Skill4 on!")));
	}

}

void UPP_MainWidgetFrame::Skill5_2()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		APP_HighPriestess* tempp = Cast<APP_HighPriestess>(temp->GetControlCharacter());
		if (tempp)
		{
			if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
				return;
			tempp->Skill5();
			////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Pristess Skill5 on!")));
		}
	}
}

void UPP_MainWidgetFrame::Skill1_3()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetSkillTarget(1, false);
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard Skill1 on!")));
	}
	else
	{
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard target skill error!")));
	}
}

void UPP_MainWidgetFrame::Skill2_3()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		APP_Wizard* tempp = Cast<APP_Wizard>(temp->GetControlCharacter());
		if (tempp)
		{
			if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
				return;
			tempp->Skill2();
			////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard Skill2 on!")));
		}
	}
}

void UPP_MainWidgetFrame::Skill3_3()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetSkillTarget(3, false);
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard Skill3 on!")));
	}
	else
	{
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard target skill error!")));
	}
}

void UPP_MainWidgetFrame::Skill4_3()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		APP_Wizard* tempp = Cast<APP_Wizard>(temp->GetControlCharacter());
		if (tempp)
		{
			if (tempp->GetPlayerInfo()->Debuff[0])//마비면 종료
				return;
			tempp->Skill4();
			////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard Skill4 on!")));
		}
	}
	else
	{
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard target skill error!")));
	}
}

void UPP_MainWidgetFrame::Skill5_3()
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(MyWorld, 0));
	if (temp->isControllCharAlive() && temp) //좌측부터 검사하니까 안튕길듯?
	{
		temp->SetSkillTarget(5, false);
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Wizard Skill5 on!")));
	}
}

UPP_QuestWidget* UPP_MainWidgetFrame::GetQuestBoard()
{
	return Quest_Board;
}

class UPP_MiniMapWidget* UPP_MainWidgetFrame::GetMiniMap()
{
	if(!MiniMapWidget)
		MiniMapWidget = Cast<UPP_MiniMapWidget>(GetWidgetFromName(TEXT("MiniMap")));
	return MiniMapWidget;
}

UPP_EquipWidgeet* UPP_MainWidgetFrame::GetEquipWidget()
{
	return EquipWidget;
}

void UPP_MainWidgetFrame::SetEquipItem(TArray<class UPP_EquipItemData*> Items)
{
	if (!InventoryWidget)
	{
		////GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Inven widget none")));
		return;
	}
	InventoryWidget->ResetEquipitem();

	////GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Item input %d"), Items.Num()));
	for (auto& item : Items)
	{
		InventoryWidget->AddEquipItem(item);
	}
}

void UPP_MainWidgetFrame::SetEquipItem(UPP_EquipItemData* Item)
{
	InventoryWidget->AddEquipItem(Item);
}

void UPP_MainWidgetFrame::ShopOpen()
{
	if(ShopWidget)
	{
		ShopWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void UPP_MainWidgetFrame::ShopClose()
{
	if (ShopWidget)
	{
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

UPP_ShopWidget* UPP_MainWidgetFrame::GetShopWidget()
{
	return ShopWidget;
}

void UPP_MainWidgetFrame::SetSkillCoolTime(int index, float per)
{
	if (index < SkillCool.Num())
	{
		if (per < 0.01)
			ReSetSkillCollTime(index);	
		else
		{
			SkillCool[index]->SetVisibility(ESlateVisibility::Visible);
			SkillCool[index]->SetPercent(per);
		}
		
	}
}

void UPP_MainWidgetFrame::ReSetSkillCollTime(int index)
{
	if (index < SkillCool.Num())
	{
		SkillCool[index]->SetVisibility(ESlateVisibility::Hidden);
	}
}