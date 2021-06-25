// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_Monster.h"
#include "PP_ArachBossAnim.h"
#include "PP_ArachBoss.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_ArachBoss : public APP_Monster
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPP_GoblinAnim* MyAnim;

public:
	APP_ArachBoss();
	UPP_ArachBossAnim* MyAnim;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
