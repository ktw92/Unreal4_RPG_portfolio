// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP_DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_DamageWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class UImage*> damage;
public:
	void SetNumber(int pos, UTexture2D* port, bool isMonsterAttack);
};
