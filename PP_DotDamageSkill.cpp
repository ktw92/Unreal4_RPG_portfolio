// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_DotDamageSkill.h"
#include "Kismet/GameplayStatics.h"
#include "KTWFunc.h"
#include "PP_Monster.h"
#include "PP_Player.h"

// Sets default values
APP_DotDamageSkill::APP_DotDamageSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	My_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	My_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SetRootComponent(My_RootScene);
	My_Niagara->SetupAttachment(My_RootScene);
	My_Particle->SetupAttachment(My_RootScene);
}

// Called when the game starts or when spawned
void APP_DotDamageSkill::BeginPlay()
{
	Super::BeginPlay();
	if (My_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), My_Sound, GetActorLocation());
}

// Called every frame
void APP_DotDamageSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	second_tick += DeltaTime;
	if (second_tick >= Dot_time)//시간마다 주변에 대미지
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
					//시체는 안건드림
					if (HittedMonster->GetStatus()->Hp <= 0)
					{
						//HittedMonster->ChangeAnimState(AnimType::Death);
						continue;
					}
					float temp_monstr_hp = HittedMonster->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					//도트 대미지 이펙트까지이펙트까지
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
					//시체는 안건드림
					if (HittedMonster->GetPlayerInfo()->Hp <= 0)
					{
						//HittedMonster->ChangeAnimState(AnimType::Death);
						continue;
					}
					float temp_monstr_hp = HittedMonster->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					//도트 대미지 이펙트까지이펙트까지
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

void APP_DotDamageSkill::SetStatus(float lifeTime, float damage, float radius, float dot_time, ECollisionChannel type, ACharacter* owner, TSubclassOf<class APP_EffectBase>* dotHitEffect)
{
	LifeTime = lifeTime;
	Damage = damage;
	Radius = radius;
	Type = type;
	Owner = owner;
	Dot_time = dot_time;
	DotHitEffect = *dotHitEffect;
}

