// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
//#include "DestructibleComponent.h"
#include "CollisionQueryParams.h"
#include "Engine.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/DataTable.h"


FHitResult DetectSphere(FVector Loc, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug = false);
FHitResult ForwardBoxOneDetect(FVector Loc, FVector Forward, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug = false);
TArray<FHitResult> ForwardBoxOneDetectMulti(FVector Loc, FVector Forward, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug = false);
TArray<FHitResult> BoxDetectMulti(FVector Loc, FVector Forward, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug = false);
TArray<FHitResult> SphereMulti(FVector Loc, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug = false);


class ELFPALADINTEMPLATE_API KTWFunc
{
private:
	KTWFunc()
	{
		;
	};
};

