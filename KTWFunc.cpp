// Fill out your copyright notice in the Description page of Project Settings.


#include "KTWFunc.h"

FHitResult DetectSphere(FVector Loc, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug)
{
	
	FHitResult	result;
	FCollisionQueryParams	params(NAME_None, false, owner);
	params.bReturnPhysicalMaterial = true;

	bool bSweep = World->SweepSingleByChannel(result, Loc,
		Loc, FQuat::Identity,
		Channel,
		FCollisionShape::MakeSphere(Range),
		params);
	/*
	if (debug)
	{
#if ENABLE_DRAW_DEBUG
		FColor	DrawColor = bSweep ? FColor::Red : FColor::Green;

		DrawDebugSphere(World, Loc, Range, 20,
			DrawColor, false, 0.3f);
#endif // ENABLE_DRAW_DEBUGEDITOR
	}
	*/
	return result;
}

FHitResult ForwardBoxOneDetect(FVector Loc, FVector Forward, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug)
{

	FHitResult	result;
	FCollisionQueryParams	params(NAME_None, false, owner);
	params.bReturnPhysicalMaterial = true;

	bool bSweep = World->SweepSingleByChannel(result, Loc + Forward * Range / 2,
		Loc + Forward * Range / 2, Forward.ToOrientationQuat(),
		Channel,
		FCollisionShape::MakeBox(FVector(Range/2, Range/2, Range/2)),
		params);
	/*
	if (debug)
	{
#if ENABLE_DRAW_DEBUG
		FColor	DrawColor = bSweep ? FColor::Red : FColor::Green;

		DrawDebugBox(World, Loc + Forward * Range / 2, FVector(Range/2 , Range/2, Range/2), Forward.ToOrientationQuat(),
			DrawColor, false, 1.f);
#endif
	}
	*/
	return result;
}

TArray<FHitResult> ForwardBoxOneDetectMulti(FVector Loc, FVector Forward, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug)
{
	TArray<FHitResult> rst;
	FHitResult	result;
	FCollisionQueryParams	params(NAME_None, false, owner);
	params.bReturnPhysicalMaterial = true;
	bool bSweep = World->SweepMultiByChannel(rst, Loc + Forward * Range / 2,
		Loc + Forward * Range / 2, Forward.ToOrientationQuat(),
		Channel,
		FCollisionShape::MakeBox(FVector(Range/2, Range/2, Range/2)),
		params);
	/*
	if (debug)
	{
#if ENABLE_DRAW_DEBUG
		FColor	DrawColor = bSweep ? FColor::Red : FColor::Green;

		DrawDebugBox(World, Loc + Forward * Range / 2, FVector(Range / 2, Range / 2, Range / 2), Forward.ToOrientationQuat(),
			DrawColor, false, 1.f);
#endif
	}
	*/
	return rst;
}

TArray<FHitResult> BoxDetectMulti(FVector Loc, FVector Forward, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug)
{
	TArray<FHitResult> rst;
	FHitResult	result;
	FCollisionQueryParams	params(NAME_None, false, owner);
	params.bReturnPhysicalMaterial = true;
	bool bSweep = World->SweepMultiByChannel(rst, Loc - Forward * Range,
		Loc + Forward * Range, Forward.ToOrientationQuat(),
		Channel,
		FCollisionShape::MakeBox(FVector(Range, Range, Range)),
		params);
	/*
	if (debug)
	{
#if ENABLE_DRAW_DEBUG
		FColor	DrawColor = bSweep ? FColor::Red : FColor::Green;

		DrawDebugBox(World, Loc, FVector(Range , Range, Range ), Forward.ToOrientationQuat(),
			DrawColor, false, 1.f);
#endif
	}
	*/
	return rst;
}

TArray<FHitResult> SphereMulti(FVector Loc, float Range, UWorld* World, ACharacter* owner, ECollisionChannel Channel, bool debug)
{

	TArray<FHitResult>	result;
	FCollisionQueryParams	params(NAME_None, false, owner);
	params.bReturnPhysicalMaterial = true;

	bool bSweep = World->SweepMultiByChannel(result, Loc,
		Loc, FQuat::Identity,
		Channel,
		FCollisionShape::MakeSphere(Range),
		params);
	/*
	if (debug)
	{
#if ENABLE_DRAW_DEBUG
		FColor	DrawColor = bSweep ? FColor::Red : FColor::Green;

		DrawDebugSphere(World, Loc, Range, 20,
			DrawColor, false, 0.3f);
#endif // ENABLE_DRAW_DEBUGEDITOR
	}
	*/
	return result;
}