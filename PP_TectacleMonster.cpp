// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_TectacleMonster.h"

#include "KTWFunc.h"
#include "PP_Player.h"



// Sets default values
APP_TectacleMonster::APP_TectacleMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	//메시 초기화
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Boris_monster/Mesh/SK_Boris_monster.SK_Boris_monster'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->AddRelativeLocation(FVector(30.f, 0, -90.f));
	

	//애니메이션 초기화
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/Tentacle/PP_TentacleMMonsterAnimBP.PP_TentacleMMonsterAnimBP_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
		
	//기본공격 이펙트
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/Goblin/PP_GoblinNormalAttackHitEffect.PP_GoblinNormalAttackHitEffect_C'"));
	if (EffectAsset.Succeeded())
		NormalHitEffect = EffectAsset.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Monsters/BoomSpyder/PP_BoomSpiderDeathEffect.PP_BoomSpiderDeathEffect_C'"));
	if (EffectAsset2.Succeeded())
		BoomEffect = EffectAsset2.Class;

	//임시
	MonsterStatus.Hp = MonsterStatus.Max_hp = 20000;
	MonsterStatus.Attack_range = 150;
	MonsterStatus.Attack[0] = 4000;
	MonsterStatus.Defense[0] = 3000;
	MonsterStatus.Name = FString::Printf(TEXT("TectacleMonster"));
	MonsterStatus.Name_name = FName("TectacleMonster");
	MonsterStatus.Exp = 1000;
	MonsterStatus.Type = MonsterType::TectacleMonster;

	//임시
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void APP_TectacleMonster::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
//	GetMesh()->SetCollisionProfileName("NoCollision");

	MyAnim = Cast<UPP_TentacleMonsterAnim>(GetMesh()->GetAnimInstance());

}

// Called every frame
void APP_TectacleMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


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
			TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), MonsterStatus.Attack_range * 30, GetWorld(), this, ECC_GameTraceChannel4, false);
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

void APP_TectacleMonster::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_TectacleMonster::NormalAttack()
{
	Super::NormalAttack();
	FHitResult temp = ForwardBoxOneDetect(this->GetActorLocation(), GetActorForwardVector(), MonsterStatus.Attack_range, GetWorld(), this, ECC_GameTraceChannel4, true);
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



		FDamageEvent	DmgEvent;
		HittedPlayer->TakeDamage(MonsterStatus.Attack[0], DmgEvent, GetController(), this);

		//랜덤으로 상태이상
		FRandomStream Rs;
		Rs.GenerateNewSeed();
		int temp_rand = Rs.FRandRange(0.f, 100);
		if (temp_rand > 70)
		{
			if (temp_rand % 2 == 0)
				HittedPlayer->SetPoison();
			else
				HittedPlayer->Paralysis();
		}	
	}
}

void APP_TectacleMonster::NormalAttack2()
{
	Super::NormalAttack2();
	FHitResult temp = ForwardBoxOneDetect(this->GetActorLocation(), GetActorForwardVector(), MonsterStatus.Attack_range, GetWorld(), this, ECC_GameTraceChannel4, true);
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



		FDamageEvent	DmgEvent;
		int amount = HittedPlayer->TakeDamage(MonsterStatus.Attack[0] * 2, DmgEvent, GetController(), this);
		MonsterStatus.Hp += amount;
	}
}

void APP_TectacleMonster::DeathBoom()
{
	TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), MonsterStatus.Attack_range * 1.2, GetWorld(), this, ECC_GameTraceChannel4, true);

	if (BoomEffect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<APP_EffectBase>(BoomEffect, GetActorLocation(),
			FRotator::ZeroRotator, param);
	}

	for (auto& hitted : temp)
	{
		APP_Player* Hitted = Cast<APP_Player>(hitted.GetActor());
		if (Hitted)
		{
			if (Hitted->GetPlayerInfo()->Hp <= 0)
			{
				continue;
			}

			FDamageEvent	DmgEvent;
			float temp_monstr_hp = Hitted->TakeDamage(MonsterStatus.Attack[0] * 2, DmgEvent, GetController(), this);
			if (Hitted == m_Target)
				if (temp_monstr_hp < 0)
					m_Target = nullptr;
		}
	}

	Destroy();
}