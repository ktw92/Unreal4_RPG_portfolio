// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_MainGameModeBase.h"

APP_MainGameModeBase::APP_MainGameModeBase()
{
	//위젯설정
	static ConstructorHelpers::FClassFinder<UPP_MainWidgetFrame> WidgetClass(TEXT("WidgetBlueprint'/Game/0_PP/PP_UI/PP_BPMainWidgetFrame.PP_BPMainWidgetFrame_C'"));

	if (WidgetClass.Succeeded())
		MainWidgetClass = WidgetClass.Class;

}

void APP_MainGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsValid(MainWidgetClass))
	{
		MainWidget = Cast<UPP_MainWidgetFrame>(CreateWidget(GetWorld(), MainWidgetClass));
		if (MainWidget)
		{
			MainWidget->AddToViewport();
			MainWidget->SetWorld(GetWorld());
		}
	}
}

void APP_MainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//임시 속도 조절
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
}