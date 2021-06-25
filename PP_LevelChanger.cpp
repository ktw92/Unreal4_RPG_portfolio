// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_LevelChanger.h"
#include "PP_PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APP_LevelChanger::APP_LevelChanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(MyBox);
	MyBox->SetCollisionProfileName("DropItem");
}

void APP_LevelChanger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APP_LevelChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APP_LevelChanger::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("map change")));
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp)
	{
		temp->CharacterSave(TargetLevel, true);
		UGameplayStatics::OpenLevel(GetWorld(), TargetLevel);
	}
	Destroy();
}

void APP_LevelChanger::LevelChangeOverlap()
{

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("map change start")));
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp)
	{
		temp->CharacterSave(TargetLevel, true);
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("char save")));
		UGameplayStatics::OpenLevel(GetWorld(), TargetLevel);
	}
	Destroy();
}
