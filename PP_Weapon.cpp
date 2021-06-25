// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Weapon.h"

APP_Weapon::APP_Weapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	My_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(My_RootScene);

	My_Mesh->SetupAttachment(My_RootScene);

	My_weaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	My_weaponEffect->SetupAttachment(My_Mesh);
}

// Called when the game starts or when spawned
void APP_Weapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APP_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APP_Weapon::EffectOnOff(bool On)
{
	if (On)
		My_weaponEffect->SetActive(true);
	else
		My_weaponEffect->SetActive(false);
}

