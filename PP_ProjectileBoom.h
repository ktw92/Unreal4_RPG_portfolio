// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "PP_ProjectileBoom.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_ProjectileBoom : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APP_ProjectileBoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isStart;
	bool isEnd;
	float endtime;

	float LifeTime;
	float LifeAcc;
	float Speed;
	float Damage;
	float Radius;
	FVector Forward;
	ECollisionChannel Type;
	ACharacter* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USoundBase* Start_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USoundBase* End_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* My_Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* My_HittedNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* My_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* My_HittedParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* My_RootScene;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetStatus(float lifeTime, float speed, float damage, float radius, FVector forward, ECollisionChannel type, ACharacter* owner);

};
