// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PP_EquipItemData.h"
#include "Blueprint/UserWidget.h"
#include "PP_EquipItemWidgeet.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_EquipItemWidgeet : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* m_Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* m_Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* m_Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* MyButton;

	UPROPERTY()
	int32	m_Index;
	UPROPERTY()
	int32	cnt;

protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	UPP_EquipItemData* MyData;

public:
	void SetItemIndex(int32 Index)
	{
		m_Index = Index;
	}

	void SetIconTexture(const FString& Path);
	void SetIconTexture(UTexture2D* pTex);
	void SetText(const FString& text);
	void SetOwner(const FString& text);

public:
	UFUNCTION(BlueprintCallable)
	void SetData(UPP_EquipItemData* data);
	UFUNCTION(BlueprintCallable)
	void EQItemClick();
};
