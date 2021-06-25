// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_DropItem.h"
#include "PP_EffectBase.h"
#include "PP_PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APP_DropItem::APP_DropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	MyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(MyBox);
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MyMesh->SetupAttachment(MyBox);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>	MeshAsset(TEXT("StaticMesh'/Game/AncientTreasures/Meshes/SM_Chest_02c.SM_Chest_02c'"));
	if (MeshAsset.Succeeded())
		MyMesh->SetStaticMesh(MeshAsset.Object);
		
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Item/PP_DropItemEffect.PP_DropItemEffect_C'"));
	if (EffectAsset.Succeeded())
		ItemEffect = EffectAsset.Class;

	MyMesh->SetCollisionProfileName("NoCollision");
	MyBox->SetCollisionProfileName("DropItem");
	MyBox->OnComponentBeginOverlap.AddDynamic(this, &APP_DropItem::BoxBeginOverlap);
}

// Called when the game starts or when spawned
void APP_DropItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APP_DropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APP_DropItem::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp)
	{
		temp->ObtainItem(ItemIndex);
		if (ItemEffect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(ItemEffect, GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
	}
	Destroy();
}

