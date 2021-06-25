// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_LevelStreamTrigger.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APP_LevelStreamTrigger::APP_LevelStreamTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(MyBox);
	MyBox->SetCollisionProfileName("Box");
	MyBox->OnComponentBeginOverlap.AddDynamic(this, &APP_LevelStreamTrigger::BoxBeginOverlap);
	MyBox->SetCollisionProfileName("DropItem");
}

// Called when the game starts or when spawned
void APP_LevelStreamTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APP_LevelStreamTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APP_LevelStreamTrigger::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, LatentInfo);
	Destroy();
}

