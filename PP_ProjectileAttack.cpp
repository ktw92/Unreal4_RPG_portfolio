// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ProjectileAttack.h"
#include "KTWFunc.h"
#include "PP_Monster.h"
#include "PP_Player.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APP_ProjectileAttack::APP_ProjectileAttack()
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
void APP_ProjectileAttack::BeginPlay()
{
	Super::BeginPlay();

	if (Start_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Start_Sound, GetActorLocation());
}

// Called every frame
void APP_ProjectileAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isEnd)
	{
		endtime += DeltaTime;
		if (endtime >= 0.7f)
			Destroy();
		return;
	}
	else if (isStart)
	{
		//충돌감지 후 터짐
		FHitResult temp = DetectSphere(GetActorLocation(), Radius, GetWorld(), Owner, Type);
		if (Type == ECC_GameTraceChannel3)//플레이어의 공격
		{
			APP_Monster* isMon = Cast<APP_Monster>(temp.Actor);
			if (isMon)
			{
				if (isMon->GetStatus()->Hp > 0) //시체가 아니면
				{
					FDamageEvent	DmgEvent;
					UGameplayStatics::GetPlayerController(GetWorld(), 0);
					isMon->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					isEnd = true;
				}
			}
		}
		else //(Type == ECC_GameTraceChannel4);
		{
			APP_Player* isP = Cast<APP_Player>(temp.Actor);
			if (isP)
			{
				//대미지 주고
				FDamageEvent	DmgEvent;
				UGameplayStatics::GetPlayerController(GetWorld(), 0);
				isP->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
				isEnd = true;
			}
		}

		//대미지 줬으면 후처리효과 및 제거활동
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
		}

		//이동
		FVector next = GetActorLocation() + Forward * DeltaTime * Speed;
		SetActorLocation(next);
	}

}

void APP_ProjectileAttack::SetStatus(float lifeTime, float speed, float damage, float radius, FVector forward, ECollisionChannel type, ACharacter* owner)
{
	LifeTime = lifeTime;
	Speed = speed;
	Damage = damage;
	Radius = radius;
	Forward = forward;
	Type = type;
	Owner = owner;

	isStart = true;
}



