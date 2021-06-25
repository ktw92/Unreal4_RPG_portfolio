// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Wizard.h"
#include "PP_ProjectileAttack.h"
#include "PP_PlayerController.h"
#include "PP_MainWidgetFrame.h"
#include "PP_MainGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PP_WizardAnim.h"
#include "KTWFunc.h"
#include "PP_Monster.h"
#include "PP_DotDamageSkill.h"

APP_Wizard::APP_Wizard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Elf_Paladin/Meshes/ElfPaladin_FullArmor.ElfPaladin_FullArmor'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	//애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_WizardAnimBP.PP_WizardAnimBP_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	//장착무기
	static ConstructorHelpers::FClassFinder<APP_Weapon>	WE(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/WizardWeapon.WizardWeapon_C'"));
	if (WE.Succeeded())
		WeaponWithEffect = WE.Class;

	//기본공격 투사체
	static ConstructorHelpers::FClassFinder<APP_ProjectileAttack>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_NormalAttack.PP_NormalAttack_C'"));
	if (EffectAsset.Succeeded())
		NormalAttackProj = EffectAsset.Class;

	//스킬1 단일개체에 폭발 및 스플래시
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset1(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_WizardSkill1.PP_WizardSkill1_C'"));
	if (EffectAsset1.Succeeded())
		Skill1Effect = EffectAsset1.Class;

	//스킬2 장판기
	static ConstructorHelpers::FClassFinder<APP_DotDamageSkill>	EffectAsset2_0(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_WizardSkill2.PP_WizardSkill2_C'"));
	if (EffectAsset2_0.Succeeded())
		Skill2Effect = EffectAsset2_0.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_WizardSkill2Hit.PP_WizardSkill2Hit_C'"));
	if (EffectAsset2.Succeeded())
		Skill2HitEffect = EffectAsset2.Class;


	//스킬3 디버프
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset3(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_WizardSkill3.PP_WizardSkill3_C'"));
	if (EffectAsset3.Succeeded())
		Skill3Effect = EffectAsset3.Class;

	//스킬4 범위전체기
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset4(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_WizardSkill4.PP_WizardSkill4_C'"));
	if (EffectAsset4.Succeeded())
		Skill4Effect = EffectAsset4.Class;

	//스킬5 단일개체 큰대미지 마력흡수
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset5(TEXT("Blueprint'/Game/0_PP/0_Player/Wizard_Paladin/PP_WizardSkill5.PP_WizardSkill5_C'"));
	if (EffectAsset5.Succeeded())
		Skill5Effect = EffectAsset5.Class;

	GetMesh()->SetCollisionProfileName("NoCollision");

	PlayerInfo.Job = FString::Printf(TEXT("Wizard"));

	//비긴플레이에서 옮겨옴
	/*
	//컨트롤로에 추가, 포트레잇UI 등록
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
		PlayerInfo.PortraitNum = temp_controller->PlayerInsert(this);
	//법사는 마지막
	PlayerInfo.PortraitNum = 3;
	*/
}

void APP_Wizard::BeginPlay()
{
	Super::BeginPlay();
	//캡슐에 맞게 메시 위치,회전조절
	GetMesh()->AddRelativeLocation(FVector(0, 0, -90.f)); //메시 위치에 따른 이동오류 -> 블루프린트로 세부조절 대체왜? -> 무기충돌체
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	MyAnim = Cast<UPP_WizardAnim>(GetMesh()->GetAnimInstance());

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
			TEXT("hand_rSocket_Wizard"));

		HighPriestessWeapon->EffectOnOff(false);
	}

	
	//컨트롤로에 추가, 포트레잇UI 등록
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
		PlayerInfo.PortraitNum = temp_controller->PlayerInsert(this);
	//법사는 마지막
	PlayerInfo.PortraitNum = 3;
	

	//데이터 읽어오기
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
			PlayerInfo.Attack_range = 2000.f;
			PlayerInfo.Attack[0] = 300;
			PlayerInfo.Defense[0] = 0;
			PlayerInfo.Hp = PlayerInfo.Max_hp = 1500;
			PlayerInfo.Mp = PlayerInfo.Max_mp = 3000;
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
				MyPortraitWidget->SetExpPercent(PlayerInfo.Exp / 5000.f);
				MyPortraitWidget->SetHpPercent(PlayerInfo.Hp / (float)(PlayerInfo.Max_hp));
				MyPortraitWidget->SetMpPercent(PlayerInfo.Mp / (float)(PlayerInfo.Max_mp));
			}
		}
	}
}

// Called every frame
void APP_Wizard::Tick(float DeltaTime)
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
		case 5: Skill5(); break;
		default:; //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("pristess skill num error")));
		}
	}
}

void APP_Wizard::SimpleMove(FVector roc)
{
	Super::SimpleMove(roc);
}
void APP_Wizard::Guard(bool isParty, bool isOn)
{
	Super::Guard(isParty, isOn);
}
void APP_Wizard::NormalAttack()
{
	Super::NormalAttack();
}

void APP_Wizard::NormalAttackFire()
{
	NormalAttack();

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));


	//발사
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	APP_ProjectileAttack* temp = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
		FRotator::ZeroRotator, param);
	temp->SetStatus(10, 1500, PlayerInfo.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel3, this);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("fire")));
}

void APP_Wizard::SetIdle()
{
	Super::SetIdle();
	SKillTargetFree();
}

void APP_Wizard::Skill1()
{
	Super::Skill1();
}

void APP_Wizard::Skill1Fire()
{
	if (!UseMp(300))
	{
		SetIdle();
		return;
	}


	if (SkillTarget)
	{
		//1차 타겟에게대미지
		APP_Monster* HittedMonsterFirst = Cast<APP_Monster>(SkillTarget);
		if (HittedMonsterFirst)
		{
			FDamageEvent	DmgEvent;
			SkillTarget->TakeDamage(PlayerInfo.Attack[0] * 2, DmgEvent,  GetController(), this);
			if (Skill1Effect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<APP_EffectBase>(Skill1Effect, HittedMonsterFirst->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}

			//2차 스플래시대미지 거리비율에 따른 대미지 추가 필요
			TArray<FHitResult> temp = SphereMulti(SkillTarget->GetActorLocation(), 200, GetWorld(), this, ECC_GameTraceChannel3, true);
			for (auto& hitted : temp)
			{
				APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
				if (HittedMonster)
				{
					//시체는 안건드림
					if (HittedMonster->GetStatus()->Hp <= 0)
					{
						//HittedMonster->ChangeAnimState(AnimType::Death);
						continue;
					}
					float temp_monstr_hp = HittedMonster->TakeDamage(PlayerInfo.Attack[0], DmgEvent, GetController(), this);
					//타겟을 죽였으면 갱신
					if (TargetedMonster)
					{
						if (HittedMonster == TargetedMonster)
							if (temp_monstr_hp < 0)
								TargetFree();
					}
				}
			}
		}
	}
}

void APP_Wizard::Skill2()
{
	Super::Skill2();
}

void APP_Wizard::Skill2Fire()
{
	if (!UseMp(100))
	{
		SetIdle();
		return;
	}

	if (Skill2Effect)
	{
		if (Skill2HitEffect)
		{
			GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
			FVector foward = GetMesh()->GetForwardVector();
			GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APP_DotDamageSkill* temp = GetWorld()->SpawnActor<APP_DotDamageSkill>(Skill2Effect, GetActorLocation() + foward * 300,
				FRotator::ZeroRotator, param);
			temp->SetStatus(7, PlayerInfo.Attack[0], 250, 0.5f, ECC_GameTraceChannel3, this, &Skill2HitEffect);
		}
	}
}

void APP_Wizard::Skill3()
{
	Super::Skill3();
}

void APP_Wizard::Skill3Fire()
{
	if (!UseMp(500))
	{
		SetIdle();
		return;
	}
	APP_Monster* HittedMonsterFirst = Cast<APP_Monster>(SkillTarget);
	if (HittedMonsterFirst)
	{
		HittedMonsterFirst->SetParalysis();

		if (Skill3Effect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(Skill3Effect, HittedMonsterFirst->GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
	}
}

void APP_Wizard::Skill4()
{
	Super::Skill4();
}

void APP_Wizard::Skill4Fire()
{
	if (!UseMp(700))
	{
		SetIdle();
		return;
	}
	
	TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), PlayerInfo.Attack_range * 2, GetWorld(), this, ECC_GameTraceChannel3, true);

	for (auto& hitted : temp)
	{
		APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
		if (HittedMonster)
		{
			//시체는 안건드림
			if (HittedMonster->GetStatus()->Hp <= 0)
			{
				//HittedMonster->ChangeAnimState(AnimType::Death);
				continue;
			}

			if (Skill4Effect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<APP_EffectBase>(Skill4Effect, HittedMonster->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}


			FDamageEvent	DmgEvent;

			float DamageAmount = PlayerInfo.Attack[0];
			//int tempHp = HittedMonster->GetStatus()->Hp; //마나흡수용

			float temp_monstr_hp = HittedMonster->TakeDamage(DamageAmount * 5, DmgEvent, GetController(), this);

			//타겟을 죽였으면 갱신
			if (TargetedMonster)
			{
				if (HittedMonster == TargetedMonster)
					if (temp_monstr_hp < 0)
						TargetFree();
			}
			//마나흡수
			/*
			int tempDmg = tempHp - temp_monstr_hp;
			if (tempDmg > 0)
			{
				PlayerInfo.Mp += tempDmg;
				if (PlayerInfo.Mp >= PlayerInfo.Max_mp)
					PlayerInfo.Mp = PlayerInfo.Max_mp;

				if (MyPortraitWidget)
					MyPortraitWidget->SetMpPercent(PlayerInfo.Mp * 1.0 / PlayerInfo.Max_mp);
			}
			*/
		}
	}

}

void APP_Wizard::Skill5()
{
	Super::Skill5();
}

void APP_Wizard::Skill5Fire()
{
	if (!UseMp(1000))
	{
		SetIdle();
		return;
	}
	if (!UseMp(300))
	{
		SetIdle();
		return;
	}


	if (SkillTarget)
	{
		//공격 후 대미지만큼 마나회복
		APP_Monster* HittedMonsterFirst = Cast<APP_Monster>(SkillTarget);
		if (HittedMonsterFirst)
		{
			FDamageEvent	DmgEvent;
			int dmg = HittedMonsterFirst->GetStatus()->Hp;
			SkillTarget->TakeDamage(PlayerInfo.Attack[0] * 10, DmgEvent, GetController(), this);
			dmg = dmg - HittedMonsterFirst->GetStatus()->Hp;
			if (Skill5Effect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<APP_EffectBase>(Skill5Effect, HittedMonsterFirst->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}
			MpUp(dmg);
		}
	}

}

void APP_Wizard::Death()
{
	Super::Death();
}
void APP_Wizard::Reraise()
{
	Super::Reraise();
	ChangeAnimState(AnimType::Reraise);
}
void APP_Wizard::Paralysis()
{
	ChangeAnimState(AnimType::Paralysis); SetIdle(); PlayerInfo.Debuff[0] = true;
	Super::Paralysis();
}
void APP_Wizard::UsingItem(ConsumeItemType type, APP_Player* target)
{
	Super::UsingItem(type, target);
}

void APP_Wizard::LevelUP()
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
		PlayerInfo.Max_hp += 1000;
		PlayerInfo.Max_mp += 1000;
		PlayerInfo.Attack[0] += 1200;
		PlayerInfo.Defense[0] += 300;
		PlayerInfo.Hp = PlayerInfo.Max_hp;
		PlayerInfo.Mp = PlayerInfo.Max_mp;
	}
}