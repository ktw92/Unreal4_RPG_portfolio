// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_PlayerController.h"
#include "PP_MiniMapIcon.h"
#include "GameFramework/Actor.h"
#include "PP_MiniMapCamOwner.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_MiniMapCamOwner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_MiniMapCamOwner();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool isMap3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Map3z;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APP_PlayerController* Controller;
	UPROPERTY()
	float delay_time;
	//미니맵 아이콘용 
	TSubclassOf<UPP_MiniMapIcon> MonsterIcon;
	TSubclassOf<UPP_MiniMapIcon> PlayerIcon;
	TArray<UPP_MiniMapIcon*> IconWidgets;

	class UPP_MiniMapWidget* MiniMap;

	void setIcon(FVector mid, FVector pos, bool isPlayer = true);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
