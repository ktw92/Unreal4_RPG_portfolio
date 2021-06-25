// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_DamageWidget.h"
#include "Components/Image.h"
#include "Widgets/SWidget.h"

void UPP_DamageWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	damage.Add(Cast<UImage>(GetWidgetFromName(TEXT("Pos_0"))));
	damage.Add(Cast<UImage>(GetWidgetFromName(TEXT("Pos_1"))));
	damage.Add(Cast<UImage>(GetWidgetFromName(TEXT("Pos_2"))));
	damage.Add(Cast<UImage>(GetWidgetFromName(TEXT("Pos_3"))));
}

void UPP_DamageWidget::SetNumber(int pos, UTexture2D* img, bool isMonsterAttack)
{
	damage[pos]->SetBrushFromTexture(img);
	if (isMonsterAttack)
	{
		damage[pos]->ColorAndOpacity = FLinearColor(1, 0, 0, .8f);
	}
	else
	{
		damage[pos]->ColorAndOpacity = FLinearColor(1, 1, 1, .8f);
	}
}