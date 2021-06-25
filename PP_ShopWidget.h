// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP_ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_ShopWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPROPERTY()
		class UButton* XButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UListView* ShopListView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* MyGold;
	UPROPERTY()
		class UPP_ShopItemData* SaleItem1;
	UPROPERTY()
		class UPP_ShopItemData* SaleItem2;
	UPROPERTY()
		FString GoldString;


protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();

	UFUNCTION(BlueprintCallable)
		void XBT();

public:
	void AddItem(class UPP_ShopItemData* item);
	void ResetEquipitem();
	void OpenShop();
	void SetGold();
};
