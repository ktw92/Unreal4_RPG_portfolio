// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ProjectileBoom.h"
#include "KTWFunc.h"
#include "PP_Monster.h"
#include "PP_Player.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APP_ProjectileBoom::APP_ProjectileBoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	My_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	My_HittedNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HittedNiagara"));
	My_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	My_HittedParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HittedParticle"));
	SetRootComponent(My_RootScene);
	My_Niagara->SetupAttachment(My_RootScene);
	My_Particle->SetupAttachment(My_RootScene);

	//My_HittedNiagara->SetAutoActivate(false);
	//My_HittedParticle->SetAutoActivate(false);

	My_HittedNiagara->bAutoActivate = false;

	My_HittedParticle->bAutoActivate = false;
	My_HittedParticle->bAutoDestroy = true;

	endtime = 0.f;
}

// Called when the game starts or when spawned
void APP_ProjectileBoom::BeginPlay()
{
	Super::BeginPlay();

	if (Start_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Start_Sound, GetActorLocation());
}

// Called every frame
void APP_ProjectileBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isEnd)
	{
		endtime += DeltaTime;
		if (endtime >= 1.5f)
			Destroy();
		return;
	}
	else if (isStart)
	{
		//시한폭탄
		LifeAcc += DeltaTime;
		if (LifeAcc >= LifeTime)
		{
			isEnd = true;
			isStart = false;
		}


		//터짐
		if (isEnd)
		{
			if (End_Sound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), End_Sound, GetActorLocation());

			My_Particle->SetActive(false);
			My_Particle->SetWorldScale3D(FVector(0, 0, 0));
			My_Niagara->SetActive(false);
			My_Niagara->SetWorldScale3D(FVector(0, 0, 0));
			if (My_HittedNiagara)
			{
				My_HittedNiagara->ActivateSystem();
				My_HittedNiagara->AttachTo(My_RootScene);
				My_HittedNiagara->Activate(true);
				My_HittedNiagara->SetActive(true);
			}
			if (My_HittedParticle)
			{
				My_HittedParticle->ActivateSystem();
				My_HittedParticle->AttachTo(My_RootScene);
				My_HittedParticle->Activate(true);
				My_HittedParticle->SetActive(true);
				//GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Green, FString::Printf(TEXT("boom11")));
			}

			TArray<FHitResult> temp = SphereMulti(GetActorLocation(), Radius, GetWorld(), Owner, Type);
			for (auto& hitted : temp)
			{
				if (Type == ECC_GameTraceChannel3)//플레이어의 공격
				{
					APP_Monster* isMon = Cast<APP_Monster>(hitted.Actor);
					if (isMon)
					{
						if (isMon->GetStatus()->Hp > 0) //시체가 아니면
						{
							FDamageEvent	DmgEvent;
							UGameplayStatics::GetPlayerController(GetWorld(), 0);
							isMon->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
						}
					}
				}
				else //(Type == ECC_GameTraceChannel4);
				{
					APP_Player* isP = Cast<APP_Player>(hitted.Actor);
					if (isP)
					{
						//대미지 주고
						FDamageEvent	DmgEvent;
						UGameplayStatics::GetPlayerController(GetWorld(), 0);
						isP->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					}
				}
			}
		}

		//이동
		FVector next = GetActorLocation() + Forward * DeltaTime * Speed;
		SetActorLocation(next);
	}

}

void APP_ProjectileBoom::SetStatus(float lifeTime, float speed, float damage, float radius, FVector forward, ECollisionChannel type, ACharacter* owner)
{
	LifeTime = lifeTime;
	Speed = speed;
	Damage = damage;
	Radius = radius;
	Forward = forward;
	Type = type;
	Owner = owner;
	LifeAcc = 0;
	isStart = true;
}

