// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Cannon.h"
#include "GameFramework/Character.h"
#include "PP_ProjectileAttack.h"

// Sets default values
APP_Cannon::APP_Cannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(My_RootScene);

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MyMesh->SetupAttachment(My_RootScene);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/0_PP/0_Object/SM_Gargoyle_Statue_On_Stand.SM_Gargoyle_Statue_On_Stand'"));
	if (MeshAsset.Succeeded())
		MyMesh->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<APP_ProjectileAttack>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Object/PP_BullteBP.PP_BullteBP_C'"));
	if (EffectAsset.Succeeded())
		BulletEffect = EffectAsset.Class;

	Tags.Add("Cannon");

	isOn = false;

}

// Called when the game starts or when spawned
void APP_Cannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APP_Cannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	time_acc += DeltaTime;
	if (time_acc > IntervalTime && isOn)
	{
		time_acc = 0;
		ShootBullet();
	}
}


void APP_Cannon::ShootBullet()
{
	if (BulletEffect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APP_ProjectileAttack* temp = GetWorld()->SpawnActor<APP_ProjectileAttack>(BulletEffect, GetActorLocation(),
			FRotator::ZeroRotator, param);

		if(!cannon_character)
			cannon_character = NewObject<ACharacter>(temp, ACharacter::StaticClass());

		if (cannon_character)
		{
			cannon_character->SetActorLocation(GetActorLocation() + FVector(0,0,300.f));
			MyMesh->AddRelativeRotation(FRotator(0.f, 90.f, 0.f));
			temp->SetStatus(4.f, 1000, Damage, 80, MyMesh->GetForwardVector(), ECC_GameTraceChannel4, cannon_character);
			MyMesh->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));
		}
	}
}
