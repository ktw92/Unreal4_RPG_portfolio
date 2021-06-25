// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Mandrake.h"
#include "KTWFunc.h"
#include "PP_Player.h"
#include "PP_MandrakeAnim.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PuzzleWall.h"
#include "PP_Cannon.h"

APP_Mandrake::APP_Mandrake()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/ROG_Creatures/Stickman/Meshes/SK_Stickman.SK_Stickman'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/Mandrake/PP_MandrakeAnimBP.PP_MandrakeAnimBP_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	MonsterStatus.Hp = MonsterStatus.Max_hp = 100;
	MonsterStatus.Attack[0] = 0;
	MonsterStatus.Defense[0] = 0;
	MonsterStatus.Name = FString::Printf(TEXT("Mandrake"));
	MonsterStatus.Name_name = FName("Mandrake");

	isNotStart = true;
}

void APP_Mandrake::BeginPlay()
{
	Super::BeginPlay();
	MyAnim = Cast<UPP_MandrakeAnim>(GetMesh()->GetAnimInstance());
}

void APP_Mandrake::ChangeAnimState(AnimType type)
{
	Super::ChangeAnimState(type);
	MyAnim->SetAnimState(type);
}

void APP_Mandrake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	time += DeltaTime;
	if (time > TimeInteval && MonsterStatus.Hp>0)
	{
		time = 0;
		ChangeAnimState(AnimType::Skill1);
	}

	if (isNotStart)
	{
		TArray<FHitResult> temp = SphereMulti(GetActorLocation(), 3800, GetWorld(), this, ECC_GameTraceChannel4, true);
		for (auto& hitted : temp)
		{
			APP_Player* Hitted = Cast<APP_Player>(hitted.GetActor());
			if (Hitted)
			{
				isNotStart = false;
				
				TArray<AActor*> Actors;

				UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Cannon", Actors);

				for (auto& canon : Actors)
				{
					APP_Cannon* temp2 = Cast<APP_Cannon>(canon);
					if (temp2)
						temp2->isOn = true;
				}
			}
		}
	}
}

void APP_Mandrake::SkillFire()
{
	TArray<FHitResult> temp = SphereMulti(GetActorLocation(), 3700, GetWorld(), this, ECC_GameTraceChannel4, true);
	for (auto& hitted : temp)
	{
		APP_Player* Hitted = Cast<APP_Player>(hitted.GetActor());
		if (Hitted)
		{
			if (Hitted->GetPlayerInfo()->Hp <= 0)
			{
				continue;
			}

			Hitted->Paralysis();
		}
	}
}

void APP_Mandrake::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "BreakWall", Actors);

	for (auto& Wall : Actors)
	{
		APP_PuzzleWall* temp = Cast<APP_PuzzleWall>(Wall);
		if (temp)
			temp->BreakMesh();
	}

	TArray<AActor*> Actors2;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Cannon", Actors2);

	for (auto& canon : Actors2)
	{
		APP_Cannon* temp = Cast<APP_Cannon>(canon);
		if (temp)
			temp->isOn = false;
	}

	TArray<AActor*> Actors3;
	//UGameplayStatics::GetAllActors(GetWorld(), "WallDecal", Actors3);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADecalActor::StaticClass(), Actors3);
	for (auto& Wall : Actors3)
	{
		Wall->Destroy();
	}
}

float APP_Mandrake::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (FVector::Distance(DamageCauser->GetActorLocation(), GetActorLocation()) > 200)
		return 0;
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return 0;
}