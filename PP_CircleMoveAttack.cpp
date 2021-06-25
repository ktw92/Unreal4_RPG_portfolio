// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_CircleMoveAttack.h"
#include "Kismet/GameplayStatics.h"
#include "KTWFunc.h"
#include "PP_Monster.h"
#include "PP_Player.h"

// Sets default values
APP_CircleMoveAttack::APP_CircleMoveAttack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	My_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	My_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SetRootComponent(My_RootScene);
	My_Niagara->SetupAttachment(My_RootScene);
	My_Particle->SetupAttachment(My_RootScene);
	Round_tick = 0.f;
}

// Called when the game starts or when spawned
void APP_CircleMoveAttack::BeginPlay()
{
	Super::BeginPlay();
	if (My_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), My_Sound, GetActorLocation());
}

// Called every frame
void APP_CircleMoveAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ȸ�������� ó��
	Round_tick += DeltaTime * 6;
	FVector next = Owner->GetActorLocation();
	Owner_r += DeltaTime * 15;
	float y = FMath::Sin(Round_tick) * Owner_r;
	float x = FMath::Cos(Round_tick) * Owner_r;
	next = next + FVector(x, y, 0);
	SetActorLocation(next);

	second_tick += DeltaTime;
	if (second_tick >= Dot_time)//�ð����� �ֺ��� �����
	{
		second_tick = 0;
		FDamageEvent	DmgEvent;
		TArray<FHitResult> temp = SphereMulti(GetActorLocation(), Radius, GetWorld(), Owner, Type);
		if (Type == ECC_GameTraceChannel3)
		{
			for (auto& hitted : temp)
			{
				APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
				if (HittedMonster)
				{
					//��ü�� �Ȱǵ帲
					if (HittedMonster->GetStatus()->Hp <= 0)
					{
						//HittedMonster->ChangeAnimState(AnimType::Death);
						continue;
					}
					float temp_monstr_hp = HittedMonster->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					//��Ʈ ����� ����Ʈ��������Ʈ����
					if (DotHitEffect)
					{
						FActorSpawnParameters	param;
						param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						GetWorld()->SpawnActor<APP_EffectBase>(DotHitEffect, HittedMonster->GetActorLocation(),
							FRotator::ZeroRotator, param);
					}
				}
			}
		}
		else //(Type == ECC_GameTraceChannel4);
		{
			for (auto& hitted : temp)
			{
				APP_Player* HittedMonster = Cast<APP_Player>(hitted.GetActor());
				if (HittedMonster)
				{
					//��ü�� �Ȱǵ帲
					if (HittedMonster->GetPlayerInfo()->Hp <= 0)
					{
						//HittedMonster->ChangeAnimState(AnimType::Death);
						continue;
					}
					float temp_monstr_hp = HittedMonster->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					//��Ʈ ����� ����Ʈ��������Ʈ����
					if (DotHitEffect)
					{
						FActorSpawnParameters	param;
						param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						GetWorld()->SpawnActor<APP_EffectBase>(DotHitEffect, HittedMonster->GetActorLocation(),
							FRotator::ZeroRotator, param);
					}
				}
			}
		}
		life_tick += Dot_time;
		if (life_tick >= LifeTime)
			Destroy();
	}
}

void APP_CircleMoveAttack::SetStatus(float lifeTime, float damage, float radius, float dot_time, ECollisionChannel type, ACharacter* owner, TSubclassOf<class APP_EffectBase>* dotHitEffect, float owner_r, float plustime)
{
	LifeTime = lifeTime;
	Damage = damage;
	Radius = radius;
	Type = type;
	Owner = owner;
	Dot_time = dot_time;
	DotHitEffect = *dotHitEffect;
	Owner_r = owner_r;
	Round_tick = plustime;
}
