// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_OrcSupporter.h"
#include "PP_ProjectileAttack.h"
#include "KTWFunc.h"
#include "PP_EffectBase.h"


APP_OrcSupporter::APP_OrcSupporter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Orc_Militant/Mesh/SK_Orc_Militant_2.SK_Orc_Militant_2'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetMesh()->AddRelativeLocation(FVector(0, 0, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/OrcSupporter/PP_OrcSuporterAnimBp.PP_OrcSuporterAnimBp_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);


	static ConstructorHelpers::FClassFinder<APP_ProjectileAttack>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/OrcSupporter/PP_OrcSupotNormalAttack.PP_OrcSupotNormalAttack_C'"));
	if (EffectAsset.Succeeded())
		NormalAttackProj = EffectAsset.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Monsters/OrcSupporter/PP_OrcSkill1Effect.PP_OrcSkill1Effect_C'"));
	if (EffectAsset2.Succeeded())
		Skill1Effect = EffectAsset2.Class;



	//임시설정 조절필요
	MonsterStatus.Attack_range = 1000.f;
	MonsterStatus.Skill_cooltime[0] = 7.f;
	MonsterStatus.Hp = MonsterStatus.Max_hp = 10000;
	MonsterStatus.Attack[0] = 800;
	MonsterStatus.Defense[0] = 500;
	MonsterStatus.Name = FString::Printf(TEXT("Orc"));
	SkillCoolTime = 0;
	MonsterStatus.Name_name = FName("Orc");
	MonsterStatus.Exp = 2000;
	MonsterStatus.Type = MonsterType::Orc;
}

void APP_OrcSupporter::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
//	GetMesh()->SetCollisionProfileName("NoCollision");
	MyAnim = Cast<UPP_OrcSupporterAnim>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void APP_OrcSupporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//스킬부분
	if (isSkillCast)
	{
		GetController()->StopMovement();
		return;
	}
	SkillCoolTime += DeltaTime;
	if (SkillCoolTime >= MonsterStatus.Skill_cooltime[0])
	{
		SkillCoolTime = 0;
		Skill1();
		return;
	}


	if (MonsterStatus.Hp <= 0 || MonsterStatus.Debuff[0] == true)
	{
		//Super::ChangeAnimState(AnimType::Death);
		GetController()->StopMovement();
		return;
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
						SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
						MyAnim->SetAnimState(AnimType::Attack1);
					}
				}
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none target")));
			GetController()->StopMovement();
			if (MyAnim)
				MyAnim->SetAnimState(AnimType::Idle);
		}
	}
	
}

void APP_OrcSupporter::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_OrcSupporter::NormalAttack()
{
	Super::NormalAttack();
	/*
	FHitResult temp = ForwardBoxOneDetect(this->GetActorLocation(), GetActorForwardVector(), AttackRange, GetWorld(), this, ECC_GameTraceChannel4, true);
	APP_Player* HittedPlayer = Cast<APP_Player>(temp.GetActor());
	if (HittedPlayer)
	{
		if (NormalHitEffect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(NormalHitEffect, HittedPlayer->GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
		}


		FDamageEvent	DmgEvent;
		HittedPlayer->TakeDamage(MonsterStatus.Attack[0], DmgEvent, GetController(), this);
	}
	*/
}

void APP_OrcSupporter::NormalAttackFire()
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));


	//발사
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	APP_ProjectileAttack* temp = GetWorld()->SpawnActor<APP_ProjectileAttack>(NormalAttackProj, GetActorLocation() + GetMesh()->GetForwardVector() * 30,
		FRotator::ZeroRotator, param);
	temp->SetStatus(10, 800, MonsterStatus.Attack[0], 50, GetMesh()->GetForwardVector(), ECC_GameTraceChannel4, this);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

}

void APP_OrcSupporter::Death()
{
	Super::Death();
}

void APP_OrcSupporter::Paralysis()
{
	Super::Paralysis();
}

void APP_OrcSupporter::Idle()
{
	Super::Idle();
}

float APP_OrcSupporter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float fDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return fDamage;
}

void APP_OrcSupporter::Skill1Fire()
{
	TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), MonsterStatus.Attack_range * 3, GetWorld(), this, ECC_GameTraceChannel3, true);
	float minHP = MonsterStatus.Hp / (float)(MonsterStatus.Max_hp);
	APP_Monster* healTarget = this;
	for (auto& hitted : temp)
	{
		APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
		if (HittedMonster)
		{
			//시체는 안건드림
			if (HittedMonster->GetStatus()->Hp <= 0)
				continue;
		}

		//체력비율로 측정함
		float temp_min = HittedMonster->GetStatus()->Hp / (float)HittedMonster->GetStatus()->Max_hp;
		if (temp_min < minHP)
		{
			minHP = temp_min;
			healTarget = HittedMonster;
		}
	}
	//가장 낮은 체력비를 가진 몬스터에게 힐
	if (Skill1Effect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//힐량조절 필요
		healTarget->HpUp(1000);
		GetWorld()->SpawnActor<APP_EffectBase>(Skill1Effect, healTarget->GetActorLocation(),
			FRotator::ZeroRotator, param);
	}
}

void APP_OrcSupporter::Skill1()
{
	Super::Skill1();
	ChangeAnimState(AnimType::Skill1);
}