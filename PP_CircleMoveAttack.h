// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "PP_CircleMoveAttack.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_CircleMoveAttack : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APP_CircleMoveAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USoundBase* My_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* My_Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* My_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* My_RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class APP_EffectBase> DotHitEffect;

	float second_tick;
	float life_tick;
	float Dot_time;
	float Damage;
	float Radius;
	ECollisionChannel Type;
	ACharacter* Owner;
	float Owner_r;
	float Round_tick;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStatus(float lifeTime, float damage, float radius, float dot_time, ECollisionChannel type, ACharacter* owner, TSubclassOf<class APP_EffectBase>* dotHitEffect, float owner_r, float plustime = 0);

};
