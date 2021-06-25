// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ArachnidBoss.h"
#include "PP_ArachBossSkill1.h"
#include "KTWFunc.h"
#include "PP_Player.h"
#include "PP_EffectBase.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PlayerController.h"
#include "PP_DotDamageSkill.h"
#include "PP_MonsterSpawnPotin.h"

// Sets default values
APP_ArachnidBoss::APP_ArachnidBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메시 초기화
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Arachnid_Boss/Mesh/SK_Arachnid_Boss.SK_Arachnid_Boss'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	//애니메이션 초기화
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/ArachnidBoss/PP_ArachnidBossAnimBP.PP_ArachnidBossAnimBP_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	//기본공격 이펙트
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/ArachnidBoss/PP_Boss2Normal1.PP_Boss2Normal1_C'"));
	if (EffectAsset.Succeeded())
		NormalHitEffect = EffectAsset.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Monsters/ArachnidBoss/PP_Boss2Normal2.PP_Boss2Normal2_C'"));
	if (EffectAsset2.Succeeded())
		Normal2HitEffect = EffectAsset2.Class;

	//스킬1
	static ConstructorHelpers::FClassFinder<APP_ArachBossSkill1> EffectAsset3(TEXT("Blueprint'/Game/0_PP/0_Monsters/ArachnidBoss/PP_Boss2Skill1.PP_Boss2Skill1_C'"));
	if (EffectAsset3.Succeeded())
		Skill1Effect = EffectAsset3.Class;

	//스킬2장판기
	static ConstructorHelpers::FClassFinder<APP_DotDamageSkill>	EffectAsset2_0(TEXT("Blueprint'/Game/0_PP/0_Monsters/ArachnidBoss/PP_BossSkill2.PP_BossSkill2_C'"));
	if (EffectAsset2_0.Succeeded())
		Skill2Effect = EffectAsset2_0.Class;

	static ConstructorHelpers::FClassFinder<APP_DropItem>	ItemAsset(TEXT("Blueprint'/Game/0_PP/0_Item/Item2.Item2_C'"));
	if (ItemAsset.Succeeded())
		Drop_Item = ItemAsset.Class;



	//임시
	MonsterStatus.Name = FString::Printf(TEXT("Arachne"));
	MonsterStatus.Name_name = FName("Arachne");
	MonsterStatus.Type = MonsterType::Arachne;

	MonsterStatus.Attack_range = 500.f;
	MonsterStatus.Skill_cooltime[0] = 7.f;
	MonsterStatus.Skill_cooltime[1] = 10.f;
	MonsterStatus.Skill_cooltime[2] = 15.f;
	MonsterStatus.Hp = MonsterStatus.Max_hp = 100000;
	MonsterStatus.Attack[0] = 3000;
	MonsterStatus.Defense[0] = 4000;
	isSkillCast = false;
	MonsterStatus.Exp = 10000;

	SkillCoolTime[0] = SkillCoolTime[1] = SkillCoolTime[2] = 0.f;
}

// Called when the game starts or when spawned
void APP_ArachnidBoss::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
	//GetMesh()->SetCollisionProfileName("NoCollision");

	MyAnim = Cast<UPP_ArachnidBossAnim>(GetMesh()->GetAnimInstance());
	ChangeAnimState(AnimType::Skill3);
}

// Called every frame
void APP_ArachnidBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	


	if (MonsterStatus.Hp <= 0 || MonsterStatus.Debuff[0] == true)
	{
		//Super::ChangeAnimState(AnimType::Death);
		GetController()->StopMovement();
		return;
	}

	if (!m_Target)
	{
		TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), 10000, GetWorld(), this, ECC_GameTraceChannel4, false);
		for (auto& hitted : temp)
		{
			APP_Player* temhit = Cast< APP_Player>(hitted.GetActor());
			if (temhit)
			{
				if (temhit->GetPlayerInfo()->Hp > 0)
				{
					m_Target = Cast<AActor>(hitted.GetActor());
					break;
				}
			}
		}
	}

	if (isSkillCast)
	{
		return;
	}
	else
	{
		for (int i = 0; i < 3; i++)
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
				}
				return;
			}
		}
	}

	if(m_Target)
	{
		float dist = FVector::Distance(m_Target->GetActorLocation(), GetActorLocation());
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("%f"),dist));
		if (dist <= 300) // 근거리 공격
		{
			if (MonsterStatus.AnimState != AnimType::Attack1)
			{
				FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
				Dir.Normalize();
				SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
				ChangeAnimState(AnimType::Attack1);
			}
		}
		else if (dist >= 8000) // 너무 멀면 몬스터 소환
		{
			if (MonsterStatus.AnimState != AnimType::Skill3)
			{
				FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
				Dir.Normalize();
				SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
				ChangeAnimState(AnimType::Skill3);
			}
		}
		else //원거리 공격
		{
			if (MonsterStatus.AnimState != AnimType::Attacl2)
			{
				FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
				Dir.Normalize();
				SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
				ChangeAnimState(AnimType::Attacl2);
			}
		}
	}

	
}

void APP_ArachnidBoss::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_ArachnidBoss::Attack1Fire()
{
	//x,y로 거리측정 + 각도로 범위내에 있으면 타격

	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		TArray<APP_Player*>* players = temp_controller->GetMyParty();

		if (players)
		{
			for (auto& player : *players)
			{
				if (player)
				{
					//방향각도
					FVector montop_forward = player->GetActorLocation() - GetActorLocation();
					
					GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
					FVector forward = GetMesh()->GetForwardVector();
					GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

					float dot = ((montop_forward.X * forward.X) + (montop_forward.Y * forward.Y))
						/ FMath::Sqrt(montop_forward.X * montop_forward.X + montop_forward.Y * montop_forward.Y)
						/ FMath::Sqrt(forward.X * forward.X + forward.Y * forward.Y);

					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("dot %f"),dot));
					
					if (0.85f < dot)
					{
						float p_x = FMath::Abs(player->GetActorLocation().X - GetActorLocation().X);
						float p_y = FMath::Abs(player->GetActorLocation().Y - GetActorLocation().Y);

						float dist = p_x + p_y;
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("dist %f"), dist));
						if (dist < 400)
						{

							//시체는 안건드림
							if (player->GetPlayerInfo()->Hp <= 0)
							{
								//HittedMonster->ChangeAnimState(AnimType::Death);
								continue;
							}

							if (NormalHitEffect)
							{
								FActorSpawnParameters	param;
								param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

								GetWorld()->SpawnActor<APP_EffectBase>(NormalHitEffect, player->GetActorLocation(),
									FRotator::ZeroRotator, param);
							}
							else
							{
								//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
							}

							FDamageEvent	DmgEvent;

							float temp_hp = player->TakeDamage(MonsterStatus.Attack[0], DmgEvent, GetController(), this);
							if (player == m_Target)
								if (temp_hp < 0)
									m_Target = nullptr;
							//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("player attack1 hit"), dist));
						}
					}
				}
			}
		}
	}

	
}
void APP_ArachnidBoss::Attack2Fire()
{
	//공격범위 * 5 검색, 가장가까운 캐릭터공격
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		TArray<APP_Player*>* players = temp_controller->GetMyParty();
		AActor* Target = nullptr;
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
						Target = player;
					}
				}
			}
		}
		if (Target)
		{
			if (Normal2HitEffect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				GetWorld()->SpawnActor<APP_EffectBase>(Normal2HitEffect, Target->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
			}

			FDamageEvent	DmgEvent;

			float temp_hp = Target->TakeDamage(MonsterStatus.Attack[0], DmgEvent, GetController(), this);
			if (Target == m_Target)
				if (temp_hp < 0)
					m_Target = nullptr;
		}
	}
}
void APP_ArachnidBoss::Skill1Fire()
{
	//가장 먼 거리의 플레이어에게 공격
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		TArray<APP_Player*>* players = temp_controller->GetMyParty();
		AActor* Target = nullptr;
		float maxdist = 0;
		if (players)
		{
			for (auto& player : *players)
			{
				if (player)
				{
					float temp_dist = FVector::Distance(GetActorLocation(), player->GetActorLocation());
					if (temp_dist > maxdist)
					{
						//시체는 안건드림
						if (player->GetPlayerInfo()->Hp <= 0)
						{
							//HittedMonster->ChangeAnimState(AnimType::Death);
							continue;
						}
						maxdist = temp_dist;
						Target = player;
					}
				}
			}
		}
		if (Target)
		{
			if (Skill1Effect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				FVector Loc = FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y, Target->GetActorLocation().Z - 50.f);
	
				APP_ArachBossSkill1* pSkill = GetWorld()->SpawnActor<APP_ArachBossSkill1>(Skill1Effect, Loc,
				FRotator::ZeroRotator, param);
				pSkill->SetOwner(this);
			}
		}
	}
}
void APP_ArachnidBoss::Skill2Fire(bool right)
{
	//우측45도 좌측45도 장판 생성
	FVector foward;
	FVector real_foward;
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	real_foward = GetMesh()->GetForwardVector();
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	if (right)//우측부터
	{
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 120.f, 0.f));
		foward = GetMesh()->GetForwardVector();
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 120.f, 0.f));
	}
	else
	{
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 60.f, 0.f));
		foward = GetMesh()->GetForwardVector();
		GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 60.f, 0.f));
	}


	if (Skill2Effect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APP_DotDamageSkill* temp = GetWorld()->SpawnActor<APP_DotDamageSkill>(Skill2Effect, GetActorLocation() + foward * 200 + real_foward * 120,
				FRotator::ZeroRotator, param);
		temp->SetStatus(7, MonsterStatus.Attack[0] * 1, 170, 0.4f, ECC_GameTraceChannel4, this, &NormalHitEffect);
	}
}
void APP_ArachnidBoss::Skill3Fire()
{
	//몬스터 스폰포인트의 리스폰횟수 올려줌
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

void APP_ArachnidBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<APP_DropItem>(Drop_Item, GetActorLocation(),
		FRotator::ZeroRotator, param);
}