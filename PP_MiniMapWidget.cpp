// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_MiniMapWidget.h"
#include "Components/PanelWidget.h"
#include "Widgets/SWidget.h"

void UPP_MiniMapWidget::NativePreConstruct()
{
	MyPanel = Cast<UPanelWidget>(GetWidgetFromName(TEXT("MyPanel")));
}

class UPanelWidget* UPP_MiniMapWidget::GetPanel()
{
	if(!MyPanel)
		MyPanel = Cast<UPanelWidget>(GetWidgetFromName(TEXT("MyPanel")));
	return MyPanel;
}