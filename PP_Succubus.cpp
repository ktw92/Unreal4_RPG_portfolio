// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Succubus.h"
#include "KTWFunc.h"
#include "PP_Player.h"
#include "PP_EffectBase.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PlayerController.h"
#include "PP_DotDamageSkill.h"
#include "PP_MonsterSpawnPotin.h"
#include "PP_ProjectileAttack.h"



// Sets default values
APP_Succubus::APP_Succubus()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�޽� �ʱ�ȭ
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/DaemonGirl/Meshes/DaemonGirl_FullClothes.DaemonGirl_FullClothes'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	//�ִϸ��̼� �ʱ�ȭ
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/Succbus/DaemonGirl_AnimBP1.DaemonGirl_AnimBP1_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	//�⺻���� ����Ʈ
	static ConstructorHelpers::FClassFinder<APP_ProjectileAttack>	EffectAsset1(TEXT("Blueprint'/Game/0_PP/0_Monsters/OrcSupporter/PP_OrcSupotNormalAttack.PP_OrcSupotNormalAttack_C'"));
	if (EffectAsset1.Succeeded())
		NormalAttackProj = EffectAsset1.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/Succbus/PP_SuccbusSKill1Effect.PP_SuccbusSKill1Effect_C'"));
	if (EffectAsset.Succeeded())
		MySkill1Effect = EffectAsset.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Monsters/Succbus/PP_SuccbusSkill2Effect.PP_SuccbusSkill2Effect_C'"));
	if (EffectAsset2.Succeeded())
		MySkill2Effect = EffectAsset2.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset3(TEXT("Blueprint'/Game/0_PP/0_Monsters/Succbus/PP_SuccbusSKill3Heal.PP_SuccbusSKill3Heal_C'"));
	if (EffectAsset3.Succeeded())
		Skill3Effect = EffectAsset3.Class;
	static ConstructorHelpers::FClassFinder<APP_ProjectileAttack>	EffectAsset5(TEXT("Blueprint'/Game/0_PP/0_Monsters/Succbus/PP_SuccbusSkill5BP.PP_SuccbusSkill5BP_C'"));
	if (EffectAsset5.Succeeded())
		Skill5Proj = EffectAsset5.Class;

	//�ӽ�
	MonsterStatus.Name = FString::Printf(TEXT("Succbus"));
	MonsterStatus.Name_name = FName("Succbus");
	MonsterStatus.Type = MonsterType::Succbus;

	MonsterStatus.Attack_range = 500.f;
	MonsterStatus.Skill_cooltime[0] = 12.f;
	MonsterStatus.Skill_cooltime[1] = 15.f;
	MonsterStatus.Skill_cooltime[2] = 20.f;
	MonsterStatus.Skill_cooltime[3] = 5.f;
	MonsterStatus.Skill_cooltime[4] = 10.f;
	MonsterStatus.Skill_cooltime[5] = 10.f;
	MonsterStatus.Hp = MonsterStatus.Max_hp = 200000;
	MonsterStatus.Attack[0] = 4000;
	MonsterStatus.Defense[0] = 3000;
	isSkillCast = false;
	MonsterStatus.Exp = 5000;

	SkillCoolTime[0] = SkillCoolTime[1] = SkillCoolTime[2] = SkillCoolTime[3] = SkillCoolTime[4] = 0.f;
}

// Called when the game starts or when spawned
void APP_Succubus::BeginPlay()
{
	Super::BeginPlay();

	//��ħ ��������
	//GetMesh()->SetCollisionProfileName("NoCollision");

	MyAnim = Cast<UPP_SuccubusAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void APP_Succubus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ChangeAnimState(AnimType::Skill2);
	

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
		for (int i = 0; i < 5; i++)
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
				}
				GetController()->StopMovement();
				return;
			}
		}
	}



	if (!m_Target)
	{
		//���� ����� ĳ���͸� Ÿ������
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
							//��ü�� �Ȱǵ帲
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
		if (dist <= 1500) // ���Ÿ�
		{
			if (MonsterStatus.AnimState != AnimType::Attack1)
			{
				FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
				Dir.Normalize();
				SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
				ChangeAnimState(AnimType::Attack1);
			}
		}
		else
		{
			if (MyAnim)
				MyAnim->SetAnimState(AnimType::Move);
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
		}
	}
}

void APP_Succubus::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_Succubus::Attack1Fire()
{
	//�⺻���� = ���尡��� ĳ���� ��������, �ڽ��� ���� + -30���� ���Ÿ� ���� ����
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (NormalAttackProj)
	{
		//�߻�
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
	}
	if (m_Target)
	{
		FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
		Dir.Normalize();
		SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
	}
	m_Target = nullptr;
}

void APP_Succubus::Skill1Fire()
{
	//�÷��̾� 3���� ���� �����
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
					FRandomStream Rs;
					Rs.GenerateNewSeed();
					float dice = Rs.FRandRange(0, 10);
					if (dice < 3)
					{
						player->SetPoison();
					}
					else
					{
						player->Paralysis();
					}
					if (MySkill1Effect)
					{
						FActorSpawnParameters	param;
						param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						GetWorld()->SpawnActor<APP_EffectBase>(MySkill1Effect, player->GetActorLocation(),
							FRotator::ZeroRotator, param);
					}
				}
			}
		}
	}

	m_Target = nullptr;
}
void APP_Succubus::Skill2Fire(bool right)
{
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		TArray<APP_Player*>* players = temp_controller->GetMyParty();
		APP_Player* Target = nullptr;
		TArray<APP_Player*> temp_player_array;
		//�߰� �Ÿ��� �ִ� �÷��̾� ü�� ���
		float maxdist = 0;
		float middist = 0;
		
		if (players)
		{
			for (auto& player : *players)
			{
				if (player)
				{
					if (player->GetPlayerInfo()->Hp <= 0)
					{
						//HittedMonster->ChangeAnimState(AnimType::Death);
						continue;
					}

					float temp_dist = FVector::Distance(GetActorLocation(), player->GetActorLocation());

					if (middist < temp_dist)
					{
						if (maxdist < temp_dist)
						{
							if (maxdist == 0)
								Target = player;
							maxdist = temp_dist;
						}
						else
						{
							middist = temp_dist;
							Target = player;
						}
					}
				}
			}
			if (Target)
			{
				//������ְ� �� ��� �ϴ� �κ�
				int temp_damage = Target->GetPlayerInfo()->Max_hp * 1.0f / 3;
				Target->HpDown(temp_damage);
				HpUp(temp_damage);

				if (MySkill2Effect)
				{
					FActorSpawnParameters	param;
					param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					GetWorld()->SpawnActor<APP_EffectBase>(MySkill2Effect, Target->GetActorLocation(),
						FRotator::ZeroRotator, param);
				}

				if (Skill3Effect)
				{
					FActorSpawnParameters	param;
					param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					GetWorld()->SpawnActor<APP_EffectBase>(Skill3Effect, GetActorLocation(),
						FRotator::ZeroRotator, param);
				}
			}
		}
	}
	if (m_Target)
	{
		FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
		Dir.Normalize();
		SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
	}
	m_Target = nullptr;
}

void APP_Succubus::Skill3Fire()
{
	//�ֺ� ������ ü���� 1/5 ȸ��
	TArray<AActor*> points;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Monster", points);
	for (auto& temp : points)
	{
		APP_Monster* mob = Cast<APP_Monster>(temp);
		if (mob)
		{
			if (mob->GetStatus()->Hp > 0)
			{
				float hp = mob->GetStatus()->Max_hp;
				hp /= 5;
				mob->HpUp(hp);
			}
			if (Skill3Effect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<APP_EffectBase>(Skill3Effect, mob->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}
		}
	}
	m_Target = nullptr;
}

void APP_Succubus::Skill4Fire()
{
	//���� �� �Ÿ��� �÷��̾�ڷ� �����̵� !!���� ���� ���� �ִ��� �扞���� �ؾ���, ������ 30���� ��ġ �ٲ㺸�� ������� �ذ�?!!
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		TArray<APP_Player*>* players = temp_controller->GetMyParty();
		APP_Player * myTarget = nullptr;
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
						//��ü�� �Ȱǵ帲
						if (player->GetPlayerInfo()->Hp <= 0)
						{
							//HittedMonster->ChangeAnimState(AnimType::Death);
							continue;
						}
						maxdist = temp_dist;
						myTarget = player;
					}
				}
			}
		}
		if (myTarget)
		{
			//���ʿ� �����̵�
			myTarget->GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));
			FVector pos = myTarget->GetActorLocation();
			pos -= myTarget->GetMesh()->GetForwardVector() * 125;
			myTarget->GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
			SetActorLocation(pos);

			FVector	Dir = myTarget->GetActorLocation() - GetActorLocation();
			Dir.Normalize();
			SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}
	}
	m_Target = nullptr;
}

void APP_Succubus::Skill5Fire()
{
	//�ڽ��ֺ� �̴ϸ��׿�
	if (Skill5Proj)
	{
		//ü���� �������� �� ���� ����
		int recentHpBonus = 0;
		int max = MonsterStatus.Max_hp;
		int recenthp = MonsterStatus.Hp;
		while (max > 0)
		{
			max -= recenthp;
			recentHpBonus++;
		}

		FVector mypos = GetActorLocation();
		for (int i = 0; i < 10 + recentHpBonus; i++)
		{
			FRandomStream Rs;
			Rs.GenerateNewSeed();
			float speed = Rs.FRandRange(-150, 150);
			float randx = Rs.FRandRange(-500, 500);
			float randy = Rs.FRandRange(-500, 500);

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			APP_ProjectileAttack* temp = GetWorld()->SpawnActor<APP_ProjectileAttack>(Skill5Proj, mypos + FVector(randx, randy, 1000) ,
				FRotator::ZeroRotator, param);

			temp->SetStatus(7, 900 + speed, MonsterStatus.Attack[0] * 2, 100, FVector(0, 0, -1), ECC_GameTraceChannel4, this);
		}
	}
	m_Target = nullptr;
}

void APP_Succubus::DeathBoom()
{
	//���� ��������Ʈ�� ������Ƚ�� �÷���
	TArray<AActor*> points;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Monster", points);
	for (auto& temp : points)
	{
		APP_Monster* mob = Cast<APP_Monster>(temp);
		if (mob)
		{
			if (mob == this)//�ڽ��� �Ȼ츲
				continue;
			if (mob->GetStatus()->Hp > 0)
			{
				float hp = mob->GetStatus()->Max_hp;
				hp /= 3;
				mob->HpUp(hp);
			}
			if (Skill3Effect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<APP_EffectBase>(Skill3Effect, mob->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}
		}
	}
}