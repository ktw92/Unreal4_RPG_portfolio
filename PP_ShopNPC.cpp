// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ShopNPC.h"
#include "PP_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PP_MainWidgetFrame.h"

// Sets default values
APP_ShopNPC::APP_ShopNPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APP_ShopNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APP_ShopNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isTrade)
	{
		APP_PlayerController* target = Cast< APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (target)
		{
			float dist = FVector::Distance(GetActorLocation(), target->GetControlCharacter()->GetActorLocation());
			if (dist >= 800)
			{
				isTrade = false;
				target->GetMainWidget()->ShopClose();
			}
		}
	}
}

