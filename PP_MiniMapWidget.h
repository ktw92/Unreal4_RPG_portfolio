// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP_MiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_MiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct();
	class UPanelWidget* MyPanel;
public:
	class UPanelWidget* GetPanel();
};
