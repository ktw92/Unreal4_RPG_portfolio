// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_EffectBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APP_EffectBase::APP_EffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//My_Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	//My_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	//My_Mesh->SetupAttachment(My_RootScene);
	//My_Decal->SetupAttachment(My_RootScene);
	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	My_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	My_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SetRootComponent(My_RootScene);
	My_Niagara->SetupAttachment(My_RootScene);
	My_Particle->SetupAttachment(My_RootScene);
}

// Called when the game starts or when spawned
void APP_EffectBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);

	if(My_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), My_Sound, GetActorLocation());
	
}

// Called every frame
void APP_EffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

