// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_Monster.h"
#include "PP_Mandrake.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_Mandrake : public APP_Monster
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APP_Mandrake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class UPP_MandrakeAnim* MyAnim;
	virtual void ChangeAnimState(AnimType type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float TimeInteval;
	float time;

	bool isNotStart;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SkillFire();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
