// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Goblin.h"
#include "PP_Player.h"

// Sets default values
APP_Goblin::APP_Goblin()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메시 초기화
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Goblin/Meshes/goblin/SK_goblin.SK_goblin'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetMesh()->AddRelativeLocation(FVector(0, 0, -92.5f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//애니메이션 초기화
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/Goblin/PP_BPGoblinAnim.PP_BPGoblinAnim_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	//기본공격 이펙트
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Monsters/Goblin/PP_GoblinNormalAttackHitEffect.PP_GoblinNormalAttackHitEffect_C'"));
	if (EffectAsset.Succeeded())
		NormalHitEffect = EffectAsset.Class;

	//임시
	AttackRange = 100.f;
	DetectRange = 10000.f;
	MonsterStatus.Hp = MonsterStatus.Max_hp = 1000;
	MonsterStatus.Attack[0] = 100;
	MonsterStatus.Defense[0] = 0;
	MonsterStatus.Name = FString::Printf(TEXT("Goblin"));
	MonsterStatus.Name_name = FName("Goblin");
	MonsterStatus.Exp = 400;
	MonsterStatus.Type = MonsterType::Goblin;
}

// Called when the game starts or when spawned
void APP_Goblin::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
	//GetMesh()->SetCollisionProfileName("NoCollision");

	MyAnim = Cast<UPP_GoblinAnim>(GetMesh()->GetAnimInstance());
	
	//무기 초기화
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GoblinWeapon = GetWorld()->SpawnActor<APP_Weapon>(APP_Weapon::StaticClass(), FVector::ZeroVector,
		FRotator::ZeroRotator, param);

	FString path = FString("StaticMesh'/Game/Goblin/Meshes/goblin/bat_Static.bat_Static'");

	UStaticMesh* pMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(pMesh))
		GoblinWeapon->SetMesh(pMesh);

	GoblinWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("wpn_socket"));
}

// Called every frame
void APP_Goblin::Tick(float DeltaTime)
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
			TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), DetectRange, GetWorld(), this, ECC_GameTraceChannel4, false);
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
			if (dist > AttackRange)
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

void APP_Goblin::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_Goblin::NormalAttack()
{
	Super::NormalAttack();
	
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
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
		}


		FDamageEvent	DmgEvent;
		HittedPlayer->TakeDamage(MonsterStatus.Attack[0], DmgEvent, GetController(),this);
	}
}

void APP_Goblin::Death()
{
	Super::Death();
}

void APP_Goblin::Paralysis()
{
	Super::Paralysis();
}

void APP_Goblin::Idle()
{
	Super::Idle();
}

float APP_Goblin::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	float fDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return fDamage;
}

void APP_Goblin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GoblinWeapon)
		GoblinWeapon->Destroy();
}