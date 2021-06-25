// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PP_ShopNPC.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_ShopNPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_ShopNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isTrade;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTrade(bool trade)
	{
		isTrade = trade;
	}

};
