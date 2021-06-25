// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "PP_Weapon.generated.h"

UCLASS()
class ELFPALADINTEMPLATE_API APP_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APP_Weapon();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* My_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* My_RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* My_weaponEffect;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMesh(UStaticMesh* mesh)
	{
		My_Mesh->SetStaticMesh(mesh);
	}

	UStaticMeshComponent* GetMesh()
	{
		return My_Mesh;
	}

	void EffectOnOff(bool On);
};
