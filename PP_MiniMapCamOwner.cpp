// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_MiniMapCamOwner.h"
#include "Kismet/GameplayStatics.h"
#include "PP_Player.h"
#include "PP_MiniMapIcon.h"
#include "PP_MainGameModeBase.h"
#include "PP_MainWidgetFrame.h"
#include "PP_MiniMapWidget.h"
#include "Components/PanelWidget.h"

#define dealtime 0.1;

// Sets default values
APP_MiniMapCamOwner::APP_MiniMapCamOwner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UPP_MiniMapIcon> WidgetClass(TEXT("WidgetBlueprint'/Game/0_PP/PP_UI/MiniMap/PP_MIniMapIcon_Monster.PP_MIniMapIcon_Monster_C'"));

	if (WidgetClass.Succeeded())
		MonsterIcon = WidgetClass.Class;

	static ConstructorHelpers::FClassFinder<UPP_MiniMapIcon> WidgetClass2(TEXT("WidgetBlueprint'/Game/0_PP/PP_UI/MiniMap/PP_MIniMapIcon_Player.PP_MIniMapIcon_Player_C'"));

	if (WidgetClass2.Succeeded())
		PlayerIcon = WidgetClass2.Class;

	delay_time = dealtime;
	isMap3 = false;
}

void APP_MiniMapCamOwner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MiniMap)
	{
		if (Controller)
			if (Controller->GetControlCharacter())
			{
				FVector tempp = Controller->GetControlCharacter()->GetActorLocation();
				if(isMap3)
					SetActorLocation(FVector(tempp.X, tempp.Y, tempp.Z + Map3z));
				else
					SetActorLocation(FVector(tempp.X, tempp.Y, tempp.Z + 10000));
			}

		delay_time -= DeltaTime;
		if (delay_time <= 0)
		{
			//초기화
			if (IconWidgets.Num() != 0)
			{
				for (auto& Icons : IconWidgets)
				{
					Icons->RemoveFromParent();
				}
				IconWidgets.Reset();
			}

			//플레이어표시
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "PC", Actors);
			FVector ActorLoc = GetActorLocation();
			for (auto& player : Actors)
			{
				setIcon(ActorLoc, player->GetActorLocation());
			}
			
			//몬스터표시
			TArray<AActor*> Actors2;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Monster", Actors2);
			for (auto& mob : Actors2)
			{
				setIcon(ActorLoc, mob->GetActorLocation(), false);
			}

			delay_time = dealtime;
			////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("delay ok")));
		}
	}
	else
	{
		APP_MainGameModeBase* temp_gamemode = Cast<APP_MainGameModeBase>(GetWorld()->GetAuthGameMode());
		if (temp_gamemode)
		{
			UPP_MainWidgetFrame* MyMainWidget = Cast<UPP_MainWidgetFrame>(temp_gamemode->GetMainWidget());
			if (MyMainWidget)
			{
				MiniMap = MyMainWidget->GetMiniMap();
			}
		}
	}
}

void APP_MiniMapCamOwner::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	Controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	while (!Controller)
	{
		Controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void APP_MiniMapCamOwner::setIcon(FVector mid, FVector pos, bool isPlayer)
{
	//범위내에 있는지 판단
	float x = pos.X - mid.X;
	if (2000 <= x || x <= -2000)
		return;
	float y = pos.Y - mid.Y;
	if (2000 <= y || y <= -2000)
		return;

	//좌표환산
	x = 200 + (x / 10);
	x = 400 - x;
	y = 200 + (y / 10);


	//아이콘 표시
	if (isPlayer)
	{
		IconWidgets.Add(Cast<UPP_MiniMapIcon>(CreateWidget(MiniMap, PlayerIcon)));
		IconWidgets[IconWidgets.Num()-1]->SetRenderTransform(FWidgetTransform(FVector2D(y, x), FVector2D(0.4f, 0.4f), FVector2D(0, 0), 0));
		MiniMap->GetPanel()->AddChild(IconWidgets[IconWidgets.Num() - 1]);
	}
	else
	{
		IconWidgets.Add(Cast<UPP_MiniMapIcon>(CreateWidget(MiniMap, MonsterIcon)));
		IconWidgets[IconWidgets.Num() - 1]->SetRenderTransform(FWidgetTransform(FVector2D(y, x), FVector2D(0.4f, 0.4f), FVector2D(0, 0), 0));
		MiniMap->GetPanel()->AddChild(IconWidgets[IconWidgets.Num() - 1]);
	}
}
