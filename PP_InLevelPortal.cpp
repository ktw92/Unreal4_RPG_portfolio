// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_InLevelPortal.h"
#include "PP_Player.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APP_InLevelPortal::APP_InLevelPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(My_RootScene);

	MyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	MyBox->SetupAttachment(My_RootScene);
	My_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	My_Particle->SetupAttachment(My_RootScene);
	MyBox->SetCollisionProfileName("DropItem");
	MyBox->OnComponentBeginOverlap.AddDynamic(this, &APP_InLevelPortal::BeginOverlap);

}

// Called when the game starts or when spawned
void APP_InLevelPortal::BeginPlay()
{
	Super::BeginPlay();
//	My_Particle->SetRelativeLocation(FVector(0, 0, 0));
}

// Called every frame
void APP_InLevelPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APP_InLevelPortal::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APP_Player* player = Cast< APP_Player>(OtherActor);
	if (player)
	{
		player->SetActorLocation(GoalPos);
		player->SimpleStop();
		if (My_Sound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), My_Sound, GetActorLocation());
	}
}
