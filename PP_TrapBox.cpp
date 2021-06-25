// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_TrapBox.h"
#include "PP_ProjectileBoom.h"
#include "GameFramework/Character.h"

// Sets default values
APP_TrapBox::APP_TrapBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	MyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(MyBox);
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MyMesh->SetupAttachment(MyBox);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/AncientTreasures/Meshes/SM_Chest_02c.SM_Chest_02c'"));
	if (MeshAsset.Succeeded())
		MyMesh->SetStaticMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<APP_ProjectileBoom>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Object/PP_TrapBoom.PP_TrapBoom_C'"));
	if (EffectAsset.Succeeded())
		TrapEffect = EffectAsset.Class;

	MyMesh->SetCollisionProfileName("NoCollision");
	MyBox->SetCollisionProfileName("DropItem");
	MyBox->OnComponentBeginOverlap.AddDynamic(this, &APP_TrapBox::BoxBeginOverlap);
}

// Called when the game starts or when spawned
void APP_TrapBox::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APP_TrapBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APP_TrapBox::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TrapEffect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(TrapEffect, GetActorLocation(),
			FRotator::ZeroRotator, param);


		ACharacter* Data = NewObject<ACharacter>(temp,
			ACharacter::StaticClass());

		Data->SetActorLocation(GetActorLocation());

		temp->SetStatus(0.1f, 0, 10000, 250, FVector(0, 0, 0), ECC_GameTraceChannel4, Data);
	}
	Destroy();
}
