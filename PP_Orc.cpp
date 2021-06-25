// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Orc.h"
#include "PP_OrcAnim.h"
#include "KTWFunc.h"
#include "PP_Player.h"
#include "PP_EffectBase.h"
#include "PP_Weapon.h"

APP_Orc::APP_Orc()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Barbarian/Mesh/SK_Barbarian_Full.SK_Barbarian_Full'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/Orc_Warrior/PP_OrcAnim.PP_OrcAnim_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	static ConstructorHelpers::FClassFinder<APP_Weapon>	WE(TEXT("Blueprint'/Game/0_PP/0_Monsters/Orc_Warrior/PP_OrcWeapon.PP_OrcWeapon_C'"));
	if (WE.Succeeded())
		WeaponWithEffect = WE.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/Orc_Warrior/PP_OrcNormalHitBP.PP_OrcNormalHitBP_C'"));
	if (EffectAsset.Succeeded())
		NormalHitEffect = EffectAsset.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Monsters/Orc_Warrior/PP_OrcSkill1Effect.PP_OrcSkill1Effect_C'"));
	if (EffectAsset2.Succeeded())
		Sk1_Effect = EffectAsset2.Class;

	static ConstructorHelpers::FClassFinder<APP_DropItem>	ItemAsset(TEXT("Blueprint'/Game/0_PP/0_Item/Item1.Item1_C'"));
	if (ItemAsset.Succeeded())
		Drop_Item = ItemAsset.Class;


	GetMesh()->AddRelativeLocation(FVector(0, 0, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void APP_Orc::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
//	GetMesh()->SetCollisionProfileName("NoCollision");
	
	MyAnim = Cast<UPP_OrcAnim>(GetMesh()->GetAnimInstance());
	
	ChangeAnimState(AnimType::Idle);


	if (WeaponWithEffect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		OrcWeapon = GetWorld()->SpawnActor<APP_Weapon>(WeaponWithEffect, FVector::ZeroVector,
			FRotator::ZeroRotator, param);

		OrcWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("hand_rSocket"));

		OrcWeapon->EffectOnOff(true);
	}

	//임시설정 조절필요
	MonsterStatus.Attack_range = 250.f;
	MonsterStatus.Skill_cooltime[0] = 10.f;
	MonsterStatus.Skill_cooltime[1] = 15.f;
	MonsterStatus.Hp = MonsterStatus.Max_hp = 45000;
	MonsterStatus.Attack[0] = 900;
	MonsterStatus.Defense[0] = 600;
	MonsterStatus.Name = FString::Printf(TEXT("Orc"));
	isSkillCast = false;
	isSkill1 = false;
	SkillCoolTime[0] = SkillCoolTime[1] = 0;
	MonsterStatus.Name_name = FName("Orc");
	MonsterStatus.Exp = 6000;
	MonsterStatus.Type = MonsterType::Orc;
}

// Called every frame
void APP_Orc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (MonsterStatus.Hp <= 0 || MonsterStatus.Debuff[0] == true)
	{
		//Super::ChangeAnimState(AnimType::Death);
		GetController()->StopMovement();
		return;
	}

	//보스니까 공격중에도 회전
	if (m_Target)
	{
		FVector dir = m_Target->GetActorLocation() - GetActorLocation();
		dir.Normalize();
		GetMesh()->SetWorldRotation(FRotator(0.f, dir.Rotation().Yaw - 90, 0.f));
	}

	if (isSkillCast)
	{
		if (isSkill1)
		{
			if (!m_Target)
			{
				FHitResult temp = DetectSphere(this->GetActorLocation(), MonsterStatus.Attack_range * 10, GetWorld(), this, ECC_GameTraceChannel4, false);
				m_Target = Cast<AActor>(temp.GetActor());
			}

			if (m_Target)
			{
				if (MyAnim->GetAnimState() != AnimType::Skill1)
				{
					float dist = FVector::Dist(GetActorLocation(), m_Target->GetActorLocation());
					if (dist <= MonsterStatus.Attack_range * 0.8) //공격거리에 있으면 공격
					{
						GetController()->StopMovement();
						Skill1();
					}
					else //공격중거리에 없으면 이동
					{
						UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
					}
				}
				FVector dir = m_Target->GetActorLocation() - GetActorLocation();
				dir.Normalize();
				GetMesh()->SetWorldRotation(FRotator(0.f, dir.Rotation().Yaw - 90, 0.f));
			}
			return;
		}
		else
		{
			GetController()->StopMovement();
			return;
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			SkillCoolTime[i] += DeltaTime;
			if (SkillCoolTime[i] >= MonsterStatus.Skill_cooltime[i])
			{
				isSkillCast = true;
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("skill start")));
				SkillCoolTime[i] = 0;
				switch (i)
				{
				default:
					break;
				case 0: isSkill1 = true; break;
				case 1: Skill2(); break;
				}
				return;
			}
		}
	}


	if (MyAnim->GetAnimState() != AnimType::Attack1)
	{
		if (!m_Target)
		{
			TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), 4000, GetWorld(), this, ECC_GameTraceChannel4, false);
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
		if (m_Target)
		{
			float dist = FVector::Distance(m_Target->GetActorLocation(), GetActorLocation());
			if (dist > MonsterStatus.Attack_range)
			{
				if (MyAnim)
					MyAnim->SetAnimState(AnimType::Move);
				UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
			}
			else
			{
				GetController()->StopMovement();
				if (MyAnim)
				{
					if (MyAnim->GetAnimState() != AnimType::Attack1)
					{
						FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
						Dir.Normalize();
						SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw - 90, 0.f));
						MyAnim->SetAnimState(AnimType::Attack1);
					}
				}
			}
		}
	}
}

void APP_Orc::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_Orc::NormalAttack()
{
	Super::NormalAttack();
	

	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	TArray<FHitResult> temp = ForwardBoxOneDetectMulti(this->GetActorLocation(), GetMesh()->GetForwardVector(), 100 + MonsterStatus.Attack_range, GetWorld(), this, ECC_GameTraceChannel4, true);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	for (auto& hitted : temp)
	{
		APP_Player* hittedPlayer = Cast<APP_Player>(hitted.GetActor());
		if (hittedPlayer)
		{
			//시체는 안건드림
			if (hittedPlayer->GetPlayerInfo()->Hp <= 0)
			{
				//HittedMonster->ChangeAnimState(AnimType::Death);
				continue;
			}

			if (NormalHitEffect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				GetWorld()->SpawnActor<APP_EffectBase>(NormalHitEffect, hittedPlayer->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}


			FDamageEvent	DmgEvent;

			float temp_hp = hittedPlayer->TakeDamage(MonsterStatus.Attack[0], DmgEvent, GetController(), this);
			if (hittedPlayer == m_Target)
				if (temp_hp < 0)
					m_Target = nullptr;
		}
	}
}

void APP_Orc::Death()
{
	Super::Death();
}

void APP_Orc::Paralysis()
{
	Super::Paralysis();
}

void APP_Orc::Idle()
{
	Super::Idle();
	isSkillCast = false;
	isSkill1 = false;
}

float APP_Orc::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float fDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return fDamage;
}

void  APP_Orc::Skill1Frie()
{
	//공격 하고 대상에게 상태이상 디버프

	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	TArray<FHitResult> temp = ForwardBoxOneDetectMulti(this->GetActorLocation(), GetMesh()->GetForwardVector(), 100 + MonsterStatus.Attack_range, GetWorld(), this, ECC_GameTraceChannel4, true);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	for (auto& hitted : temp)
	{
		APP_Player* hittedPlayer = Cast<APP_Player>(hitted.GetActor());
		if (hittedPlayer)
		{
			hittedPlayer->Paralysis();
			//시체는 안건드림
			if (hittedPlayer->GetPlayerInfo()->Hp <= 0)
			{
				//HittedMonster->ChangeAnimState(AnimType::Death);
				continue;
			}

			if (Sk1_Effect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				GetWorld()->SpawnActor<APP_EffectBase>(Sk1_Effect, hittedPlayer->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}

			FDamageEvent	DmgEvent;

			float temp_hp = hittedPlayer->TakeDamage(MonsterStatus.Attack[0] * 3, DmgEvent, GetController(), this);
			if (hittedPlayer == m_Target)
				if (temp_hp < 0)
					m_Target = nullptr;
		}
	}
}

void APP_Orc::Skill2Frie()
{
	//범위 마비
	TArray<FHitResult> temp = SphereMulti(GetActorLocation(), 1500, GetWorld(), this, ECC_GameTraceChannel4, true);
	for (auto& hitted : temp)
	{
		APP_Player* Hitted = Cast<APP_Player>(hitted.GetActor());
		if (Hitted)	
		{
			if (Hitted->GetPlayerInfo()->Hp <= 0)
			{
				continue;
			}

			Hitted->Paralysis();
		}
	}
}

void APP_Orc::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OrcWeapon)
		OrcWeapon->Destroy();

	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<APP_DropItem>(Drop_Item, GetActorLocation(),
		FRotator::ZeroRotator, param);
}