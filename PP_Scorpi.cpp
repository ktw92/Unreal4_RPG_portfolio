// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Scorpi.h"
#include "KTWFunc.h"
#include "PP_Player.h"
#include "Math/UnrealMathUtility.h"

APP_Scorpi::APP_Scorpi()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//메시 초기화
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Anasimus/Mesh/SK_Anasimus.SK_Anasimus'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetMesh()->AddRelativeLocation(FVector(0, 0, -77.5f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	//애니메이션 초기화
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/Scorpi/PP_ScorpiBPAnim.PP_ScorpiBPAnim_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	//기본공격 이펙트
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/Goblin/PP_GoblinNormalAttackHitEffect.PP_GoblinNormalAttackHitEffect_C'"));
	if (EffectAsset.Succeeded())
		NormalHitEffect = EffectAsset.Class;

	//임시
	MonsterStatus.Hp = MonsterStatus.Max_hp = 10000;
	MonsterStatus.Attack_range = 200;
	MonsterStatus.Attack[0] = 3000;
	MonsterStatus.Defense[0] = 1500;
	MonsterStatus.Name = FString::Printf(TEXT("Scorpi"));
	MonsterStatus.Name_name = FName("Scorpi");
	MonsterStatus.Exp = 1700;
	MonsterStatus.Type = MonsterType::Scorpi;
	PatrolTime = 2.5f;
	PatrolAcc = 0.f;
}


// Called when the game starts or when spawned
void APP_Scorpi::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
	GetMesh()->SetCollisionProfileName("NoCollision");

	MyAnim = Cast<UPP_ScorpiAnim>(GetMesh()->GetAnimInstance());

}

// Called every frame
void APP_Scorpi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterStatus.Hp <= 0 || MonsterStatus.Debuff[0] == true)
	{
		GetController()->StopMovement();
		return;
	}


	if (m_Target)
	{
		float dist = FVector::Distance(m_Target->GetActorLocation(), GetActorLocation());
		if (dist > MonsterStatus.Attack_range)
		{
			GetCharacterMovement()->MaxWalkSpeed = 800;
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
		PatrolAcc += DeltaTime;
		if (PatrolAcc >= PatrolTime)
		{
			PatrolAcc = 0;

			float x = GetActorLocation().X + FMath::FRandRange(-1000.f, 1000.f);
			float y = GetActorLocation().Y + FMath::FRandRange(-1000.f, 1000.f);

			goal = FVector(x, y, GetActorLocation().Z);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), goal);
			if (MyAnim)
				MyAnim->SetAnimState(AnimType::Move);

			GetCharacterMovement()->MaxWalkSpeed = 300;
		}
		else
		{
			if(FVector::Dist(GetActorLocation(), goal) <= 300)
			{
				GetController()->StopMovement();
				if (MyAnim)
					MyAnim->SetAnimState(AnimType::Idle);
			}
		}
	}

}

void APP_Scorpi::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

float APP_Scorpi::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	float fDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!m_Target)//목표 대상이 없으면 떄린 플레이어를 타겟으로
	{
		m_Target = DamageCauser;
	}
	return fDamage;
}

void APP_Scorpi::NormalAttack()
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
	}
}