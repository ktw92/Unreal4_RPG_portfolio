// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_LastBoss.h"
#include "PP_Player.h"
#include "PP_EffectBase.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PlayerController.h"
#include "PP_DotDamageSkill.h"
#include "PP_ProjectileBoom.h"

APP_LastBoss::APP_LastBoss()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/0_LastBoss/AnimDemoScene/Mesh/SK_Monster_Underwater_01.SK_Monster_Underwater_01'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossAnimBP.PP_LastBossAnimBP_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	static ConstructorHelpers::FClassFinder<APP_ProjectileBoom>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSKill5.PP_SatanSKill5_C'"));
	if (EffectAsset.Succeeded())
		NormalAttackEffect = EffectAsset.Class;

	static ConstructorHelpers::FClassFinder<APP_DotDamageSkill>	EffectAsset1(TEXT("Blueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossSkill1.PP_LastBossSkill1_C'"));
	if (EffectAsset1.Succeeded())
		Skill1Effect = EffectAsset1.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset1_1(TEXT("Blueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossSkill1Hit.PP_LastBossSkill1Hit_C'"));
	if (EffectAsset1_1.Succeeded())
		Skill1_HitEffect = EffectAsset1_1.Class;


	static ConstructorHelpers::FClassFinder<APP_ProjectileBoom>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossSkill2.PP_LastBossSkill2_C'"));
	if (EffectAsset2.Succeeded())
		Skill2Effect = EffectAsset2.Class;


	static ConstructorHelpers::FClassFinder<APP_ProjectileBoom>	EffectAsset3(TEXT("Blueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossSkill3.PP_LastBossSkill3_C'"));
	if (EffectAsset3.Succeeded())
		Skill3Effect = EffectAsset3.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset4(TEXT("Blueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossSkill4.PP_LastBossSkill4_C'"));
	if (EffectAsset4.Succeeded())
		Skill4Effect = EffectAsset4.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset5(TEXT("Blueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossSkill5.PP_LastBossSkill5_C'"));
	if (EffectAsset5.Succeeded())
		Skill5Effect = EffectAsset5.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset6(TEXT("Blueprint'/Game/0_PP/0_Monsters/LastBoss/PP_LastBossSkill6.PP_LastBossSkill6_C'"));
	if (EffectAsset6.Succeeded())
		Skill6Effect = EffectAsset6.Class;


	//임시
	MonsterStatus.Name = FString::Printf(TEXT("Lavos"));
	MonsterStatus.Name_name = FName("Lavos");
	MonsterStatus.Type = MonsterType::Lavos;

	MonsterStatus.Attack_range = 500.f;
	MonsterStatus.Skill_cooltime[0] = 12.f;
	MonsterStatus.Skill_cooltime[1] = 15.f;
	MonsterStatus.Skill_cooltime[2] = 20.f;
	MonsterStatus.Skill_cooltime[3] = 18.f;
	MonsterStatus.Skill_cooltime[4] = 10.f;
	MonsterStatus.Skill_cooltime[5] = 20.f;
	MonsterStatus.Hp = MonsterStatus.Max_hp = 600000;
	MonsterStatus.Attack[0] = 8000;
	MonsterStatus.Defense[0] = 10000;
	isSkillCast = false;
	MonsterStatus.Exp = 10000;

	SkillCoolTime[0] = SkillCoolTime[1] = SkillCoolTime[2] = SkillCoolTime[3] = SkillCoolTime[4] = SkillCoolTime[5] = 0.f;
}

void APP_LastBoss::BeginPlay()
{
	Super::BeginPlay();

	MyAnim = Cast<UPP_LastBossAnim>(GetMesh()->GetAnimInstance());

}

void APP_LastBoss::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_LastBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (MonsterStatus.Hp <= 0) //|| MonsterStatus.Debuff[0] == true)
	{
		//Super::ChangeAnimState(AnimType::Death);
		GetController()->StopMovement();
		return;
	}

	if (isSkillCast)
	{
		GetController()->StopMovement();
		return;
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			SkillCoolTime[i] += DeltaTime;
			if (SkillCoolTime[i] >= MonsterStatus.Skill_cooltime[i])
			{
				isSkillCast = true;
				SkillCoolTime[i] = 0;
				switch (i)
				{
				default:
					break;
				case 0: Skill1(); break;
				case 1: Skill2(); break;
				case 2: Skill3(); break;
				case 3: Skill4(); break;
				case 4: Skill5(); break;
				case 5: Skill6(); break;
				}
				GetController()->StopMovement();
				return;
			}
		}
	}
	ChangeAnimState(AnimType::Attack1);
}

void APP_LastBoss::Attack1Fire()
{
	// 3캐릭터중 1개명의 캐릭터위치에 폭발
	if (NormalAttackEffect)
	{
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();
			FRandomStream Rs;
			Rs.GenerateNewSeed();
			int i  = Rs.FRandRange(0, 2.99f);
			APP_Player* player = nullptr;
			if((*players).Num() <= 3)
				 player = (*players)[i];

			if (player)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(NormalAttackEffect, player->GetActorLocation(),
					FRotator::ZeroRotator, param);

				temp->SetStatus(1.1f, 0, MonsterStatus.Attack[0], 225, FVector(0, 0, 0), ECC_GameTraceChannel4, this);
			}		
		}
	}
}

void APP_LastBoss::Skill1Fire()
{
	
	// 바둑판식 위치에 장판기술
	if (Skill1Effect && Skill1_HitEffect)
	{
		FRandomStream Rs;
		Rs.GenerateNewSeed();
		int isEven = Rs.FRandRange(0, 50.f);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("LastBOss Skil1 Fire OK isEven %d"),isEven));
		//판 mid x = 900 (Max 2200 min -400, 1/8 = 325) /mid y = 600 (Max1900 min -700 1/8 = 325) /z = -145
		FVector Skill1OriginPos = FVector(-175, -275, -145);
		for (int i = 0; i < 4; i++)
		{
			isEven++;
			for (int j = 0; j < 4; j++)
			{
				if (j % 2 == isEven % 2)
				{
				//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("SKill1 %d, %d"),i,j));

					FActorSpawnParameters	param;
					param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					FVector pos = FVector(i*650, j*650, 0); //로케이션 조절 필요
					{
						APP_DotDamageSkill* temp = GetWorld()->SpawnActor<APP_DotDamageSkill>(Skill1Effect, Skill1OriginPos+pos,
							FRotator::ZeroRotator, param);
						temp->SetStatus(7, MonsterStatus.Attack[0], 400, 1.f, ECC_GameTraceChannel4, this, &Skill1_HitEffect);
					}
				}
			}
		}
	}
}

void APP_LastBoss::Skill2Fire(bool IsFirst)
{
	// 특정 캐릭터 주변에 폭발(1차 주변, 2차 중심)
	if (Skill2Effect)
	{
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			if (IsFirst)
			{
				TArray<APP_Player*>* players = temp_controller->GetMyParty();
				FRandomStream Rs;
				Rs.GenerateNewSeed();
				int tempint = Rs.FRandRange(0, 2.99f);
				APP_Player* Skill2Target = nullptr;
				if ((*players).Num() <= 3)
					Skill2Target = (*players)[tempint];
				if (Skill2Target)
				{
					for (int i = 0; i < 8; i++)
					{
						Skill2Target->GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + i * 45.f, 0.f));
						APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill2Effect, Skill2Target->GetActorLocation() + Skill2Target->GetMesh()->GetForwardVector() * 400,
							FRotator::ZeroRotator, param);
						temp->SetStatus(1.1f, 0, MonsterStatus.Attack[0] * 1.5f, 225, FVector(0, 0, 0), ECC_GameTraceChannel4, this);
						GetMesh()->SetRelativeRotation(FRotator(0.f, Skill2Target->GetMesh()->GetRelativeRotation().Yaw - i * 45.f, 0.f));
					}
				}
				Skill2Targetpos = Skill2Target->GetActorLocation();
			}
			else
			{
				APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill2Effect, Skill2Targetpos,
						FRotator::ZeroRotator, param);
			}
		}
	}
}

void APP_LastBoss::Skill3Fire()
{
	// 각 사분면 메테오
	if (Skill3Effect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//판 mid x = 900 (Max 2200 min -400, 1/4 = 650) /mid y = 600 (Max1900 min -700 1/4 = 650) /z = -145
		FVector pos[4];
		pos[0] = FVector(1550, 1250, -145 + 1500);
		pos[1] = FVector(250, 1250, -145 + 2250);
		pos[2] = FVector(1550, -50, -145 + 3000);
		pos[3] = FVector(250, -50, -145 + 3750);
		for (int i = 0; i < 4; i++)
		{		
			APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill3Effect, pos[i] ,
				FRotator::ZeroRotator, param);
			temp->SetStatus(2 + i, 750, MonsterStatus.Attack[0] * 2, 1000, FVector(0, 0, -1), ECC_GameTraceChannel4, this);
		}
	}
}

void APP_LastBoss::Skill4Fire()
{
	// 캐릭터2명 마비 + 독
	if (Skill4Effect)
	{
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();
			FRandomStream Rs;
			Rs.GenerateNewSeed();
			int i = Rs.FRandRange(0, 2.99f);

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			int j = 0;
			for (auto& player : *players)
			{
				if (i == j++)
					continue;
			
				APP_EffectBase* temp = GetWorld()->SpawnActor<APP_EffectBase>(Skill4Effect, player->GetActorLocation(),
					FRotator::ZeroRotator, param);
				player->Paralysis();
				player->SetPoison();
			}
		}
	}
}

void APP_LastBoss::Skill5Fire()
{
	//단일타켓 고정10만대미지 (즉사급)
	if (Skill5Effect)
	{
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();
			FRandomStream Rs;
			Rs.GenerateNewSeed();
			int i = Rs.FRandRange(0, 2.99f);
			APP_Player* player = nullptr;
			if ((*players).Num() <= 3)
				player = (*players)[i];

			if (player)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				APP_EffectBase* temp = GetWorld()->SpawnActor<APP_EffectBase>(Skill5Effect, player->GetActorLocation(),
					FRotator::ZeroRotator, param);

				FDamageEvent dmgevent;
				player->TakeDamage(100000, dmgevent, GetController(), this);
			}
		}
	}
}

void APP_LastBoss::Skill6Fire()
{
	// 전체 피 흡수 (20%씩)
	if (Skill6Effect)
	{
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			for (auto& player : *players)
			{
		
				APP_EffectBase* temp = GetWorld()->SpawnActor<APP_EffectBase>(Skill6Effect, player->GetActorLocation(),
					FRotator::ZeroRotator, param);
				int damage = player->GetPlayerInfo()->Max_hp * 0.2f + player->GetPlayerInfo()->Defense[0];
				FDamageEvent dmgevent;
				player->TakeDamage(damage, dmgevent, GetController(), this);
				HpUp(damage);
			}
		}
	}
}
