// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP_InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API UPP_InventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPROPERTY()
	class UButton* EquipItemButton;
	UPROPERTY()
	class UButton* ConsumItemButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTileView* m_ConsumTileView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UListView* m_EquipListView;

protected:
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	
	UFUNCTION(BlueprintCallable)
		void EquipBT();
	UFUNCTION(BlueprintCallable)
		void ConsumBT();
public:
	void AddEquipItem(class UPP_EquipItemData* item);
	void ResetEquipitem();
};
