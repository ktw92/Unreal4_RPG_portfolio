// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PP_MonsterStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_MonsterStatusWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProgressBar* HpBar;
	class UTextBlock* NameText;

	class APP_PlayerController* CharacterController;

protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	//0.8초후 제거용
	UPROPERTY()
	bool isStart;
	UPROPERTY()
	bool isHiddening;
	UPROPERTY()
	float Hiddenacc;

public:
	void SetHpPercent(float Percent)
	{
		HpBar->SetPercent(Percent);
	}
	void SetHidden();
	void SetVisible();
	void SetNameText(const FString& text);
};
