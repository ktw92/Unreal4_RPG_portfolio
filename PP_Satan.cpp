// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Satan.h"
#include "PP_Player.h"
#include "PP_EffectBase.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PlayerController.h"
#include "PP_DotDamageSkill.h"
#include "PP_DotHealSkill.h"
#include "PP_MonsterSpawnPotin.h"
#include "PP_ProjectileAttack.h"
#include "PP_CircleMoveAttack.h"
#include "PP_ProjectileBoom.h"

APP_Satan::APP_Satan()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Demon_2/D2_Mesh/SK_Demon_2.SK_Demon_2'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	GetMesh()->AddRelativeLocation(FVector(0, 0, -100.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanAnimBP.PP_SatanAnimBP_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	//기본공격
	static ConstructorHelpers::FClassFinder<APP_ProjectileAttack>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_OrcSupotNormalAttack.PP_OrcSupotNormalAttack_C'"));
	if (EffectAsset.Succeeded())
		NormalAttackProj = EffectAsset.Class;

	//스킬1
	static ConstructorHelpers::FClassFinder<APP_DotDamageSkill>	EffectAsset1(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSkill1Main.PP_SatanSkill1Main_C'"));
	if (EffectAsset1.Succeeded())
		Skill1Effect = EffectAsset1.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset1_1(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSkill1Hit.PP_SatanSkill1Hit_C'"));
	if (EffectAsset1_1.Succeeded())
		Skill1_1HitEffect = EffectAsset1_1.Class;
	static ConstructorHelpers::FClassFinder<APP_DotHealSkill>	EffectAsset1_2(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSkill1Sub.PP_SatanSkill1Sub_C'"));
	if (EffectAsset1_2.Succeeded())
		Skill1_2Effect = EffectAsset1_2.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset1_2_1(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatabSkill1SubHit.PP_SatabSkill1SubHit_C'"));
	if (EffectAsset1_2_1.Succeeded())
		Skill1_2HitEffect = EffectAsset1_2_1.Class;

	//스킬2
	static ConstructorHelpers::FClassFinder<APP_CircleMoveAttack>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSkill2BP.PP_SatanSkill2BP_C'"));
	if (EffectAsset2.Succeeded())
		Skill2Effect = EffectAsset2.Class;
		static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2_2(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSkill2Hit.PP_SatanSkill2Hit_C'"));
	if (EffectAsset2_2.Succeeded())
		Skill12HitEffect = EffectAsset2_2.Class;

	//스킬3
	static ConstructorHelpers::FClassFinder<APP_ProjectileBoom>	EffectAsset3(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSkill3Main.PP_SatanSkill3Main_C'"));
	if (EffectAsset3.Succeeded())
		Skill3Effect = EffectAsset3.Class;

	//스킬4
	static ConstructorHelpers::FClassFinder<APP_ProjectileBoom>	EffectAsset4(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSkill4.PP_SatanSkill4_C'"));
	if (EffectAsset4.Succeeded())
		Skill4Effect = EffectAsset4.Class;

	//스킬5
	static ConstructorHelpers::FClassFinder<APP_ProjectileBoom>	EffectAsset5(TEXT("Blueprint'/Game/0_PP/0_Monsters/Satan/PP_SatanSKill5.PP_SatanSKill5_C'"));
	if (EffectAsset5.Succeeded())
		Skill5Effect = EffectAsset5.Class;

	static ConstructorHelpers::FClassFinder<APP_DropItem>	ItemAsset(TEXT("Blueprint'/Game/0_PP/0_Item/Item5.Item5_C'"));
	if (ItemAsset.Succeeded())
		Drop_Item = ItemAsset.Class;
	

	//임시
	MonsterStatus.Name = FString::Printf(TEXT("Satan"));
	MonsterStatus.Name_name = FName("Satan");
	MonsterStatus.Type = MonsterType::Satan;

	MonsterStatus.Attack_range = 500.f;
	MonsterStatus.Skill_cooltime[0] = 12.f;
	MonsterStatus.Skill_cooltime[1] = 15.f;
	MonsterStatus.Skill_cooltime[2] = 20.f;
	MonsterStatus.Skill_cooltime[3] = 15.f;
	MonsterStatus.Skill_cooltime[4] = 10.f;
	MonsterStatus.Skill_cooltime[5] = 20.f;
	MonsterStatus.Hp = MonsterStatus.Max_hp = 250000;
	MonsterStatus.Attack[0] = 4500;
	MonsterStatus.Defense[0] = 6000;
	isSkillCast = false;
	MonsterStatus.Exp = 5000;

	SkillCoolTime[0] = SkillCoolTime[1] = SkillCoolTime[2] = SkillCoolTime[3] = SkillCoolTime[4] = SkillCoolTime[5] = 0.f;

	Skill5dsit = 0;
}

void APP_Satan::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
	//GetMesh()->SetCollisionProfileName("NoCollision");

	MyAnim = Cast<UPP_SatanAnim>(GetMesh()->GetAnimInstance());

}

void APP_Satan::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

// Called every frame
void APP_Satan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (MonsterStatus.Hp <= 0 || MonsterStatus.Debuff[0] == true)
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
		for (int i = 0; i < 6;  i++)
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



	if (!m_Target)
	{
		//가장 가까운 캐릭터를 타겟으로
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		Idle();
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();
			float mindist = 123456789;
			if (players)
			{
				for (auto& player : *players)
				{
					if (player)
					{
						float temp_dist = FVector::Distance(GetActorLocation(), player->GetActorLocation());
						if (temp_dist < mindist)
						{
							//시체는 안건드림
							if (player->GetPlayerInfo()->Hp <= 0)
							{
								//HittedMonster->ChangeAnimState(AnimType::Death);
								continue;
							}
							mindist = temp_dist;
							m_Target = player;
						}
					}
				}
			}
		}
	}
	else
	{
		float dist = FVector::Distance(m_Target->GetActorLocation(), GetActorLocation());
		if (dist <= 1500) // 원거리
		{
			if (MonsterStatus.AnimState != AnimType::Attack1)
			{
				FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
				Dir.Normalize();
				SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
				ChangeAnimState(AnimType::Attack1);
				GetController()->StopMovement();
			}
		}
		else
		{
			if (MonsterStatus.AnimState == AnimType::Idle)
			{
				if (MyAnim)
					MyAnim->SetAnimState(AnimType::Move);
				UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
			}
		}
	}
}

// Called to bind functionality to input
void APP_Satan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APP_Satan::Attack1Fire()
{
	//기본공격 - 부채꼴브레스
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (NormalAttackProj)
	{
		//발사
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
		APP_ProjectileAttack* temp = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
			FRotator::ZeroRotator, param);
		temp->SetStatus(10, 800, MonsterStatus.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel4, this);
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 120.f, 0.f));
		APP_ProjectileAttack* temp2 = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
			FRotator::ZeroRotator, param);
		temp2->SetStatus(10, 800, MonsterStatus.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel4, this);
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 120.f, 0.f));

		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 60.f, 0.f));
		APP_ProjectileAttack* temp3 = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
			FRotator::ZeroRotator, param);
		temp3->SetStatus(10, 800, MonsterStatus.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel4, this);
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 60.f, 0.f));

		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 105.f, 0.f));
		APP_ProjectileAttack* temp4 = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
			FRotator::ZeroRotator, param);
		temp4->SetStatus(10, 800, MonsterStatus.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel4, this);
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw -  105.f, 0.f));

		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 75.f, 0.f));
		APP_ProjectileAttack* temp5 = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
			FRotator::ZeroRotator, param);
		temp5->SetStatus(10, 800, MonsterStatus.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel4, this);
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 75.f, 0.f));
	}

	m_Target = nullptr;
}

void APP_Satan::Skill1Fire()
{
	//스킬1 장판생성 후 플레이어에게 대미지 및 몬스터는 HP회복 (DotDamage, DotHeal생성)
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//대미지
	if (Skill1Effect && Skill1_1HitEffect)
	{
		APP_DotDamageSkill* temp = GetWorld()->SpawnActor<APP_DotDamageSkill>(Skill1Effect, GetActorLocation(),
			FRotator::ZeroRotator, param);
		temp->SetStatus(7, MonsterStatus.Attack[0], 450, 1.f, ECC_GameTraceChannel4, this, &Skill1_1HitEffect);
	}
	//힐
	if (Skill1_2Effect && Skill1_2HitEffect)
	{
		APP_DotHealSkill* temp2 = GetWorld()->SpawnActor<APP_DotHealSkill>(Skill1_2Effect, GetActorLocation(),
			FRotator::ZeroRotator, param);
		temp2->SetStatus(7, MonsterStatus.Attack[0], 450, 1.f, ECC_GameTraceChannel3, this, &Skill1_2HitEffect);
	}
}

void APP_Satan::Skill2Fire(bool isR)
{
	//스킬2 원형 움직임 화염구 (자신의 주변에 서클 어택 소환)  
	if (Skill2Effect && Skill12HitEffect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APP_CircleMoveAttack* temp2 = GetWorld()->SpawnActor<APP_CircleMoveAttack>(Skill2Effect, GetActorLocation(),
			FRotator::ZeroRotator, param);
		if(isR)
			temp2->SetStatus(12, MonsterStatus.Attack[0], 200, 0.5f, ECC_GameTraceChannel4, this, &Skill12HitEffect, 100);
		else
			temp2->SetStatus(12, MonsterStatus.Attack[0], 200, 0.5f, ECC_GameTraceChannel4, this, &Skill12HitEffect, 100, 7.f);
	}
}

void APP_Satan::Skill3Fire()
{
	//스킬3 거대 화염구 (내려오는 시간 = 프로젝타일 스킬 시간에 딱 맞도록)
	if (Skill3Effect)
	{
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();
			FRandomStream Rs;
			Rs.GenerateNewSeed();
			int tempint = Rs.FRandRange(0, 2.99f);
			APP_Player* SkillTarget = nullptr;
			if ((*players).Num() <= 3)
				SkillTarget = (*players)[tempint];


			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill3Effect, SkillTarget->GetActorLocation() + FVector(0, 0, 1500),
				FRotator::ZeroRotator, param);

			temp->SetStatus(2, 750, MonsterStatus.Attack[0] * 1.5f, 700, FVector(0, 0, -1), ECC_GameTraceChannel4, this);
		}
	}
}

void APP_Satan::Skill4Fire()
{
	//스킬4 미니메테오 (서큐버스 스킬 + 특정캐릭터)
	if (Skill4Effect)
	{
		//체력이 적을수록 더 많은 공격
		int recentHpBonus = 0;
		int max = MonsterStatus.Max_hp;
		int recenthp = MonsterStatus.Hp;
		while (max > 0)
		{
			max -= recenthp;
			recentHpBonus++;
		}
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();
			FRandomStream Rs;
			Rs.GenerateNewSeed();
			int tempint = Rs.FRandRange(0, 2.99f);
			APP_Player* SkillTarget = nullptr;
			if ((*players).Num() <= 3)
				SkillTarget = (*players)[tempint];

			FVector mypos = SkillTarget->GetActorLocation();
			for (int i = 0; i < 8 + recentHpBonus; i++)
			{
				Rs.GenerateNewSeed();
				float speed = Rs.FRandRange(-150, 150);
				float randx = Rs.FRandRange(-500, 500);
				float randy = Rs.FRandRange(-500, 500);

				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill4Effect, mypos + FVector(randx, randy, -80),
					FRotator::ZeroRotator, param);

				temp->SetStatus(1.1f, 0, MonsterStatus.Attack[0] * 1.2f, 200, FVector(0, 0, 0), ECC_GameTraceChannel4, this);
			}
		}
	}
}

void APP_Satan::Skill5Fire()
{
	//스킬5 자신주변 8방향에 폭발 (0.3초 생명짜리 프로젝타일 스킬 8방향으로)
	if (Skill5Effect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int i = 0; i < 8; i++)
		{
			GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + i * 45.f, 0.f));
			APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill5Effect, GetActorLocation() + GetMesh()->GetForwardVector() * Skill5dsit,
				FRotator::ZeroRotator, param);
			temp->SetStatus(1.1f, 0, MonsterStatus.Attack[0] * 1.5f, 225, FVector(0, 0, 0), ECC_GameTraceChannel4, this);
			GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw -  i * 45.f, 0.f));
		}
	}
}

void APP_Satan::Skill6Fire()
{
	//스킬6 몬스터소환
	TArray<AActor*> points;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "SpawnPoint", points);
	for (auto& temp : points)
	{
		APP_MonsterSpawnPotin* spawn = Cast<APP_MonsterSpawnPotin>(temp);
		if (spawn)
		{
			spawn->SetRespawn(0.5f, 1);
		}
	}
}

void APP_Satan::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<APP_DropItem>(Drop_Item, GetActorLocation(),
		FRotator::ZeroRotator, param);
}