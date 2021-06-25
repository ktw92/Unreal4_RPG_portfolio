// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PP_GoblinAIController.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_GoblinAIController : public AAIController
{
	GENERATED_BODY()
public:
	APP_GoblinAIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
