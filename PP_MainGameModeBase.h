// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_MainWidgetFrame.h"
#include "GameFramework/GameModeBase.h"
#include "PP_MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_MainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	APP_MainGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UPP_MainWidgetFrame> MainWidgetClass;
	UPP_MainWidgetFrame* MainWidget;

public:
	virtual void BeginPlay();
	/*
	class UNewWorldWidget* GetWidget()
	{
		return m_NewWorldWidget;
	}
	*/
	UPP_MainWidgetFrame* GetMainWidget()
	{
		return MainWidget;
	}
};
