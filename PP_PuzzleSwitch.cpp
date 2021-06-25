// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_PuzzleSwitch.h"
#include "PP_PuzzleSwitchWall.h"
#include "Kismet/GameplayStatics.h"

APP_PuzzleSwitch::APP_PuzzleSwitch()
{
	MonsterStatus.Hp = MonsterStatus.Max_hp = 10;
	MonsterStatus.Attack[0] = 0;
	MonsterStatus.Defense[0] = 10000000;
	MonsterStatus.Name = FString::Printf(TEXT("Swicth"));
	MonsterStatus.Name_name = FName("Switch");
}

float APP_PuzzleSwitch::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (FVector::Distance(DamageCauser->GetActorLocation(), GetActorLocation()) > 200)
		return 0;

	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TagName) ,Actors);
	for (auto& Wall : Actors)
	{
		APP_PuzzleSwitchWall* temp = Cast< APP_PuzzleSwitchWall>(Wall);
		if (temp)
			temp->ChangePos();
	}

	return 0;
}