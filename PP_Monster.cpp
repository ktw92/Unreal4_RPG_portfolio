// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Monster.h"
#include "PP_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PP_MonsterSpawnPotin.h"

// Sets default values
APP_Monster::APP_Monster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Monster");
	GetCapsuleComponent()->SetCollisionProfileName("Monster");
	GetMesh()->SetCustomDepthStencilValue(1);
	GetMesh()->SetRenderCustomDepth(true);
}

// Called when the game starts or when spawned
void APP_Monster::BeginPlay()
{
	Super::BeginPlay();
	//폭발제거
	GetMesh()->SetCollisionProfileName("NoColButMouseOk");
	
	MonsterStatus.Paralysis_Acc = 0;
	isSkillCast = false;
	nonedead = true;
}

// Called every frame
void APP_Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MonsterStatus.Hp <= 0)
	{
		ChangeAnimState(AnimType::Death);
		return;
	}
	else if (MonsterStatus.Debuff[0] == true)//마비상태면
	{
		MonsterStatus.Paralysis_Acc += DeltaTime;
		////GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("pala time %f / %f"), MonsterStatus.Paralysis_Acc, ParalysisTime));


		if (MonsterStatus.Paralysis_Acc >= ParalysisTime)
		{
			MonsterStatus.Paralysis_Acc = 0;
			MonsterStatus.Debuff[0] = false;
		}
	}

	//대미지 주는 곳에서 판단하면 다른 몹이 죽인거 판단 애매해짐
	if (m_Target)
	{
		APP_Player* temp_m = Cast<APP_Player>(m_Target);
		if (temp_m->GetPlayerInfo()->Hp <= 0)
			m_Target = nullptr;
	}
}

// Called to bind functionality to input
void APP_Monster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float APP_Monster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float fDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (MonsterStatus.Hp <= 0)
	{
		//ChangeAnimState(AnimType::Death);
		return (float)MonsterStatus.Hp;
	}

	float temp_dmg = DamageAmount - MonsterStatus.Defense[0];
	if (temp_dmg < 0)
		temp_dmg = 0;
	MonsterStatus.Hp -= temp_dmg;
	////GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Monster Take Damage Hp : %d"), MonsterStatus.Hp));

	if(MonsterStatus.Hp <= 0)
	{
		if (nonedead)
		{
			nonedead = false;
			APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (temp)
			{
				temp->SetMonsterHuntting(MonsterStatus.Type);
				temp->ObtainExps(MonsterStatus.Exp);
			}
		}
		Death();
	}

	return (float)MonsterStatus.Hp;
}

void APP_Monster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (MySpawnPoint)
		MySpawnPoint->Restart();
	/*
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	while(!temp)
	{
		temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	temp->ObtainExps(MonsterStatus.Exp);
	*/
}

void APP_Monster::HpUp(int amount)
{
	MonsterStatus.Hp += amount;
	if (MonsterStatus.Hp > MonsterStatus.Max_hp)
	{
		MonsterStatus.Hp = MonsterStatus.Max_hp;
	}
}