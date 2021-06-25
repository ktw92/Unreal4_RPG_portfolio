// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_BuffIconWidget.h"
#include "Components/Image.h"

void UPP_BuffIconWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	m_Icon = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));

	ticktick = false;
	Time = time_acc = ticktick_acc = 0.f;
}

void UPP_BuffIconWidget::NativeConstruct()
{
	Super::NativeConstruct();
	m_Icon->SetVisibility(ESlateVisibility::Hidden);
}

void UPP_BuffIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Time > 0)
	{
		time_acc += InDeltaTime;
		if (Time < time_acc)
		{
			ResetIcon();
		}

		if (Time - time_acc < 1)
		{
			ticktick_acc += InDeltaTime;
			if (ticktick_acc > 0.05f)
			{
				ticktick_acc = 0;
				SetIconticktick();
			}
		}
		else if (Time - time_acc < 3)
		{
			ticktick_acc += InDeltaTime;
			if (ticktick_acc > 0.15f)
			{
				ticktick_acc = 0;
				SetIconticktick();
			}
		}
	}
}

void UPP_BuffIconWidget::SetIconStart(float bftime)
{
	Time = bftime;
	time_acc = 0;
	ticktick = false;
	ticktick_acc = 0;
	SetIconticktick();
}

void UPP_BuffIconWidget::SetIconticktick()
{
	ticktick = !ticktick;
	if (ticktick)
	{
		m_Icon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_Icon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPP_BuffIconWidget::ResetIcon()
{
	ticktick = false;
	Time = time_acc = ticktick_acc = 0;
	m_Icon->SetVisibility(ESlateVisibility::Hidden);
}