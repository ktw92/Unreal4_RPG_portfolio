// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_QuestWidget.h"
#include "Components/TextBlock.h"
#include "Widgets/SWidget.h"

void UPP_QuestWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	QuestName = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestName")));
	TargetName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TargetName")));
	GoalDesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoalDesc")));
}

void UPP_QuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPP_QuestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPP_QuestWidget::SetNameText(const FString& name)
{
	QuestNameText = FText::FromString(name);
	if(QuestName)
		QuestName->SetText(QuestNameText);
}

void UPP_QuestWidget::SetNameText(const FText& name)
{
	QuestNameText = name;
	if (QuestName)
		QuestName->SetText(QuestNameText);
}

void UPP_QuestWidget::SetTargetText(const FString& name)
{
	TargetNameText = FText::FromString(name);
	if(TargetName)
		TargetName->SetText(TargetNameText);
}

void UPP_QuestWidget::SetTargetText(const FText& name)
{
	TargetNameText = name;
	if (TargetName)
		TargetName->SetText(name);
}

void UPP_QuestWidget::SetGoalDescText(const FString& name)
{
	GoalDescText = FText::FromString(name);
	if(GoalDesc)
		GoalDesc->SetText(GoalDescText);
}

void UPP_QuestWidget::SetGoalDescText(const FText& name)
{
	GoalDescText = name;
	if (GoalDesc)
		GoalDesc->SetText(name);
}