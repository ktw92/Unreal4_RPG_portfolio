// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_BoomSpider.h"
#include "KTWFunc.h"
#include "PP_Player.h"


// Sets default values
APP_BoomSpider::APP_BoomSpider()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메시 초기화
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/SpideEye/Mesh/SK_SpideEye.SK_SpideEye'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->AddRelativeLocation(FVector(30.f, 0, -82.f));
	GetMesh()->SetWorldScale3D(FVector(0.3f, 0.3f, 0.4f));


	//애니메이션 초기화
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/BoomSpyder/PP_BoomSpiderAnimBP.PP_BoomSpiderAnimBP_C'"));
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
	MonsterStatus.Hp = MonsterStatus.Max_hp = 5000;
	MonsterStatus.Attack_range = 150;
	MonsterStatus.Attack[0] = 2500;
	MonsterStatus.Defense[0] = 2000;
	MonsterStatus.Name = FString::Printf(TEXT("BoomSpider"));
	MonsterStatus.Name_name = FName("BoomSpider");
	MonsterStatus.Exp = 0;
	MonsterStatus.Type = MonsterType::BoomSpider;
	
	//임시
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void APP_BoomSpider::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
	//GetMesh()->SetCollisionProfileName("NoCollision");

	MyAnim = Cast<UPP_BoomspiderAnim>(GetMesh()->GetAnimInstance());

}

// Called every frame
void APP_BoomSpider::Tick(float DeltaTime)
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

void APP_BoomSpider::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_BoomSpider::NormalAttack()
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
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
		}


		FDamageEvent	DmgEvent;
		HittedPlayer->TakeDamage(MonsterStatus.Attack[0], DmgEvent, GetController(), this);
	}
}

void APP_BoomSpider::DeathBoom()
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
