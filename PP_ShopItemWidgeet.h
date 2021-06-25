// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PP_ShopItemData.h"
#include "Blueprint/UserWidget.h"
#include "PP_ShopItemWidgeet.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_ShopItemWidgeet : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* m_Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* m_Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* m_Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* MyButton;
	UPROPERTY()
		int32	m_Index;

protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	UPP_ShopItemData* MyData;

public:
	void SetItemIndex(int32 Index)
	{
		m_Index = Index;
	}

	void SetIconTexture(const FString& Path);
	void SetIconTexture(UTexture2D* pTex);
	void SetText(const FString& text);
	void SetPrice(const FString& text);

public:
	UFUNCTION(BlueprintCallable)
		void SetData(UPP_ShopItemData* data);
	UFUNCTION(BlueprintCallable)
		void ShopItemClick();
};
