// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_HighPriestess.h"
#include "PP_ProjectileAttack.h"
#include "PP_PlayerController.h"
#include "PP_MainWidgetFrame.h"
#include "PP_MainGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PP_ElfSwordMaster.h"
#include "PP_Wizard.h"

APP_HighPriestess::APP_HighPriestess()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/HighPriestess/Meshes/HighPriestess_FullClothes.HighPriestess_FullClothes'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	//애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Player/HighPristess/PP_HighPriestess_AnimBP.PP_HighPriestess_AnimBP_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	//장착무기
	static ConstructorHelpers::FClassFinder<APP_Weapon>	WE(TEXT("Blueprint'/Game/0_PP/0_Player/HighPristess/PP_HighPristessWeapon.PP_HighPristessWeapon_C'"));
	if (WE.Succeeded())
		WeaponWithEffect = WE.Class;

	//기본공격 투사체
	static ConstructorHelpers::FClassFinder<APP_ProjectileAttack>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Player/HighPristess/PP_NormalAttack.PP_NormalAttack_C'"));
	if (EffectAsset.Succeeded())
		NormalAttackProj = EffectAsset.Class;

	//스킬1 단일힐
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset1(TEXT("Blueprint'/Game/0_PP/0_Player/HighPristess/PP_PristessSkill1Effect.PP_PristessSkill1Effect_C'"));
	if (EffectAsset1.Succeeded())
		Skill1Effect = EffectAsset1.Class;

	//스킬2 전체힐
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Player/HighPristess/PP_PristessSkill2Effect.PP_PristessSkill2Effect_C'"));
	if (EffectAsset2.Succeeded())
		Skill2Effect = EffectAsset2.Class;

	//스킬3 단일상태이상회복
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset3(TEXT("Blueprint'/Game/0_PP/0_Player/HighPristess/PP_PristessSkill3Effect.PP_PristessSkill3Effect_C'"));
	if (EffectAsset3.Succeeded())
		Skill3Effect = EffectAsset3.Class;

	//스킬4 단일부활
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset4(TEXT("Blueprint'/Game/0_PP/0_Player/HighPristess/PP_PristessSkill4Effect.PP_PristessSkill4Effect_C'"));
	if (EffectAsset4.Succeeded())
		Skill4Effect = EffectAsset4.Class;

	//스킬5초회복
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset5(TEXT("Blueprint'/Game/0_PP/0_Player/HighPristess/PP_PristessSkill5Effect.PP_PristessSkill5Effect_C'"));
	if (EffectAsset5.Succeeded())
		Skill5Effect = EffectAsset5.Class;
	
	GetMesh()->SetCollisionProfileName("NoCollision");
	
	PlayerInfo.Job = FString::Printf(TEXT("Pristess"));

	//비긴플레이에서 옮겨옴
	/*
	//컨트롤로에 추가, 포트레잇UI 등록
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
		PlayerInfo.PortraitNum = temp_controller->PlayerInsert(this);
	//힐러는 중간
	PlayerInfo.PortraitNum = 2;
	*/
}

void APP_HighPriestess::BeginPlay()
{
	Super::BeginPlay();
	//캡슐에 맞게 메시 위치,회전조절
	GetMesh()->AddRelativeLocation(FVector(0, 0, -90.f)); //메시 위치에 따른 이동오류 -> 블루프린트로 세부조절 대체왜? -> 무기충돌체
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	MyAnim = Cast<UPP_HighPriestessAnim>(GetMesh()->GetAnimInstance());

	if (MyAnim)
		ChangeAnimState(AnimType::Idle);

	//무기장착
	if (WeaponWithEffect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		HighPriestessWeapon = GetWorld()->SpawnActor<APP_Weapon>(WeaponWithEffect, FVector::ZeroVector,
			FRotator::ZeroRotator, param);

		HighPriestessWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("hand_rSocket"));

		HighPriestessWeapon->EffectOnOff(false);
	}

	
	//컨트롤로에 추가, 포트레잇UI 등록
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
		PlayerInfo.PortraitNum = temp_controller->PlayerInsert(this);
	//힐러는 중간
	PlayerInfo.PortraitNum = 2;


	UPP_GameInstance* tempInstance = Cast<UPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (tempInstance)
	{
		if (tempInstance->IsSave())
		{
			FPP_PlayerInfo* tempInfo = tempInstance->GetPlayerInfo(PlayerInfo.PortraitNum - 1);
			if (tempInfo)
			{
				PlayerInfo.Attack_range = tempInfo->Attack_range;
				PlayerInfo.Attack[0] = tempInfo->Attack[0];
				PlayerInfo.Defense[0] = tempInfo->Defense[0];
				PlayerInfo.Hp = tempInfo->Hp;
				PlayerInfo.Max_hp = tempInfo->Max_hp;
				PlayerInfo.Mp = tempInfo->Mp;
				PlayerInfo.Max_mp = tempInfo->Max_mp;
				PlayerInfo.Exp = tempInfo->Exp;
			}
		}
		else
		{
			//플레이어 정보 초기화 임시, 나중에 전환필요
			PlayerInfo.Attack_range = 2000.f;
			PlayerInfo.Attack[0] = 100;
			PlayerInfo.Defense[0] = 20;
			PlayerInfo.Hp = PlayerInfo.Max_hp = 2000;
			PlayerInfo.Mp = PlayerInfo.Max_mp = 2000;
		}
	}

	//상태위젯과 연결
	APP_MainGameModeBase* temp_gamemode = Cast<APP_MainGameModeBase>(GetWorld()->GetAuthGameMode());
	if (temp_gamemode)
	{
		UPP_MainWidgetFrame* temp_mainwidget = Cast<UPP_MainWidgetFrame>(temp_gamemode->GetMainWidget());
		if (temp_mainwidget)
		{
			UPP_PlayerPortraitWidget* temp_portraitWidget = temp_mainwidget->GetPortraitWidget(PlayerInfo.PortraitNum);
			if (temp_portraitWidget)
			{
				MyPortraitWidget = temp_portraitWidget;
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("PortraitWidget Load complete")));
				MyPortraitWidget->SetExpPercent(PlayerInfo.Exp / 5000.f);
				MyPortraitWidget->SetHpPercent(PlayerInfo.Hp / (float)(PlayerInfo.Max_hp));
				MyPortraitWidget->SetMpPercent(PlayerInfo.Mp / (float)(PlayerInfo.Max_mp));
			}
	
		}
		
	}

}

// Called every frame
void APP_HighPriestess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerInfo.isDead)
	{
		if (PlayerInfo.AnimState != AnimType::Reraise)
			ChangeAnimState(AnimType::Death);
		GetController()->StopMovement();
		return;
	}

	if (PlayerInfo.Debuff[0] == true)//마비상태
	{
		ChangeAnimState(AnimType::Paralysis);
		return;
	}
	//타겟설정되면 스킬시전 준비
	if (SkillTarget && !isSKill)
	{
		isSKill = true;
		/*
		if (PlayerInfo.AnimState != AnimType::Skill4)
		{
			float dist = FVector::Dist(GetActorLocation(), SkillTarget->GetActorLocation());
			if (dist <= PlayerInfo.Attack_range) //스킬거리에 있으면 발동
			{
				FVector dir = SkillTarget->GetActorLocation() - GetActorLocation();
				dir.Normalize();
				GetMesh()->SetWorldRotation(FRotator(0.f, dir.Rotation().Yaw - 90.f, 0.f));
				SimpleStop();
				switch(TargetSkillNum)
				{
					case 1: Skill1(); break;
					case 3: Skill2(); break;
					case 4: Skill3(); break;
					default: GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("pristess skill num error")));
				}
			}
			else //사거거리에 없으면 이동
			{
				AActor* tempm = SkillTarget;
				SimpleMove(SkillTarget->GetActorLocation());
				SkillTarget = tempm;
			}
		}
		*/
		FVector dir = SkillTarget->GetActorLocation() - GetActorLocation();
		dir.Normalize();
		GetMesh()->SetWorldRotation(FRotator(0.f, dir.Rotation().Yaw - 90.f, 0.f));
		SimpleStop();
		switch (TargetSkillNum)
		{
		case 1: Skill1(); break;
		case 3: Skill3(); break;
		case 4: Skill4(); break;
		default:;// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("pristess skill num error")));
		}
	}
}

void APP_HighPriestess::SimpleMove(FVector roc)
{
	Super::SimpleMove(roc);
}
void APP_HighPriestess::Guard(bool isParty, bool isOn)
{
	Super::Guard(isParty, isOn);
}
void APP_HighPriestess::NormalAttack()
{
	Super::NormalAttack();
}

void APP_HighPriestess::NormalAttackFire()
{
	NormalAttack();

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));


	//발사
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	APP_ProjectileAttack* temp = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
		FRotator::ZeroRotator, param);
	temp->SetStatus(10, 1000, PlayerInfo.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel3, this);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("fire")));
}

void APP_HighPriestess::SetIdle()
{
	Super::SetIdle();
	SKillTargetFree();
}

void APP_HighPriestess::Skill1()
{
	Super::Skill1();
}

void APP_HighPriestess::Skill1Fire()
{
	if (!UseMp(100))
	{
		SetIdle();
		return;
	}
	APP_Player* player = Cast<APP_Player>(SkillTarget);
	if (player)
	{
		int amount = player->GetPlayerInfo()->Max_hp / 3;
		player->HpUp(amount);
		if (Skill1Effect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(Skill1Effect, player->GetActorLocation(),
				FRotator::ZeroRotator, param);
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("pristerss skill1 fire ok")));
		}
	}
}

void APP_HighPriestess::Skill2()
{
	Super::Skill2();
}

void APP_HighPriestess::Skill2Fire()
{
	if (!UseMp(100))
	{
		SetIdle();
		return;
	}
	
	//컨트롤러에서 플레어 목록 얻어오기
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TArray<APP_Player*>* temp_party = temp_controller->GetMyParty();
	//캐릭터들 힐 및 이펙트 생성
	for (auto& players : *temp_party)
	{
		int amount = players->GetPlayerInfo()->Max_hp / 5;
		players->HpUp(amount);

		if (Skill2Effect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(Skill2Effect, players->GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
	}
}

void APP_HighPriestess::Skill3()
{
	Super::Skill3();
}

void APP_HighPriestess::Skill3Fire()
{
	if (!UseMp(100))
	{
		SetIdle();
		return;
	}
	APP_Player* player = Cast<APP_Player>(SkillTarget);
	if (player)
	{
		//상태이상 해제 함수 필요
		if (player->GetPlayerInfo()->Debuff[0] == true)
		{
			APP_ElfSwordMaster* temp_knight = Cast<APP_ElfSwordMaster>(ItemTarget);
			APP_HighPriestess* temp_pristess = Cast<APP_HighPriestess>(ItemTarget);
			APP_Wizard* temp_wizard = Cast<APP_Wizard>(ItemTarget);
			if (temp_knight)
			{
				temp_knight->SetIdle();
			}
			else if (temp_pristess)
			{
				temp_pristess->SetIdle();
			}
			else if (temp_wizard)
			{
				temp_wizard->SetIdle();
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (player->GetPortrait())
				player->GetPortrait()->ReSetDenuff(i);
			player->GetPlayerInfo()->Debuff[i] = false;
		}
		player->GetPlayerInfo()->Paralysis_Acc = 0;
		player->GetPlayerInfo()->Poison_Acc = 0;

		if (Skill3Effect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(Skill3Effect, player->GetActorLocation(),
				FRotator::ZeroRotator, param);
			
		}
	}
}

void APP_HighPriestess::Skill4()
{
	Super::Skill4();
}

void APP_HighPriestess::Skill4Fire()
{
	if (!UseMp(100))
	{
		SetIdle();
		return;
	}
	APP_Player* player = Cast<APP_Player>(SkillTarget);
	if (player)
	{
		player->GetPlayerInfo()->Hp = player->GetPlayerInfo()->Max_hp / 2 - 100;
		player->HpUp(100);
		if (player->GetPlayerInfo()->isDead)
			player->Reraise();

		if (Skill4Effect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(Skill4Effect, player->GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
	}
}

void APP_HighPriestess::Skill5()
{
	Super::Skill5();
}

void APP_HighPriestess::Skill5Fire()
{
	if (!UseMp(500))
	{
		SetIdle();
		return;
	}

	//컨트롤러에서 플레어 목록 얻어오기
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TArray<APP_Player*>* temp_party = temp_controller->GetMyParty();
	//캐릭터들 힐 및 이펙트 생성
	for (auto& players : *temp_party)
	{
		int amount = players->GetPlayerInfo()->Max_mp;
		players->MpUp(amount);
		players->GetPlayerInfo()->Hp = players->GetPlayerInfo()->Max_hp - 100;
		players->HpUp(100);
		if(players->GetPlayerInfo()->isDead)
			players->Reraise();

		if (Skill5Effect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(Skill5Effect, players->GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
	}

}

void APP_HighPriestess::Death()
{
	Super::Death();
}
void APP_HighPriestess::Reraise()
{
	Super::Reraise();
	ChangeAnimState(AnimType::Reraise);
}
void APP_HighPriestess::Paralysis()
{
	ChangeAnimState(AnimType::Paralysis); SetIdle(); PlayerInfo.Debuff[0] = true; PlayerInfo.Paralysis_Acc = 0;
	Super::Paralysis();
}
void APP_HighPriestess::UsingItem(ConsumeItemType type, APP_Player* target)
{
	Super::UsingItem(type, target);
}

void APP_HighPriestess::LevelUP()
{
	Super::LevelUP();

	int i = 0;
	while (PlayerInfo.Exp >= 5000)
	{
		i++;
		PlayerInfo.Exp -= 5000;
	}
	while (i > 0)
	{
		i--;
		PlayerInfo.Max_hp += 1500;
		PlayerInfo.Max_mp += 1000;
		PlayerInfo.Attack[0] += 500;
		PlayerInfo.Defense[0] += 500;
		PlayerInfo.Hp = PlayerInfo.Max_hp;
		PlayerInfo.Mp = PlayerInfo.Max_mp;
	}
}