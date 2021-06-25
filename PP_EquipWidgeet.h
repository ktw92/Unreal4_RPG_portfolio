// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP_EquipWidgeet.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_EquipWidgeet : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct();
	class UTexture2D* Portrait_texture[3];
	class UTexture2D* Item_texture[6];
	class UTexture2D* LoadTextureFromPath(const FString& Path);
	class UImage* Portrait;
	class UImage* Item;
public:
	void SetImageAndVisible(int portnum, int itmnum, bool isVisible = true);
};
