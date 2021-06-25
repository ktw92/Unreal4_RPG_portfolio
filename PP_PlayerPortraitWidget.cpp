// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_PlayerPortraitWidget.h"
#include "PP_BuffIconWidget.h"
#include "Components/TextBlock.h"
#include "Widgets/SWidget.h"

void UPP_PlayerPortraitWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Hp")));
	MpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Mp")));
	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Exp")));
	JobName = Cast<UTextBlock>(GetWidgetFromName(TEXT("JobName")));
	Buff1 = Cast<UPP_BuffIconWidget>(GetWidgetFromName(TEXT("PP_SwordMasterBuffWidget")));
	DeBuff0 = Cast<UPP_BuffIconWidget>(GetWidgetFromName(TEXT("PP_Debuff0Widget")));
	DeBuff1 = Cast<UPP_BuffIconWidget>(GetWidgetFromName(TEXT("PP_Debuff1Widget")));
}

void UPP_PlayerPortraitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPP_PlayerPortraitWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPP_PlayerPortraitWidget::SetHpPercent(float Percent)
{
	HpBar->SetPercent(Percent);
}

void UPP_PlayerPortraitWidget::SetMpPercent(float Percent)
{
	MpBar->SetPercent(Percent);
}

void UPP_PlayerPortraitWidget::SetExpPercent(float Percent)
{
	ExpBar->SetPercent(Percent);
}

void UPP_PlayerPortraitWidget::SetNameText(FString name)
{
	JobName->SetText(FText::FromString(name));
}

void UPP_PlayerPortraitWidget::SetBuff1(float time)
{
	Buff1->SetIconStart(time);
}

void UPP_PlayerPortraitWidget::SetDeBuff(int index, float time)
{
	//배열로 할걸 그랬나
	if (index == 0)
		DeBuff0->SetIconStart(time);
	else
		DeBuff1->SetIconStart(time);
}

void UPP_PlayerPortraitWidget::ReSetDenuff(int index)
{
	if (index == 0)
		DeBuff0->ResetIcon();
	else
		DeBuff1->ResetIcon();
}