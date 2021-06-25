// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ArachBossAnim.h"
#include "PP_ArachBoss.h"

UPP_ArachBossAnim::UPP_ArachBossAnim()
{

	static ConstructorHelpers::FClassFinder<APP_ArachBossSkill1>	Sk1Asset(TEXT("Blueprint'/Game/0_PP/0_Monsters/ArachBoss/NewBlueprint1.NewBlueprint1_C'"));

		if (Sk1Asset.Succeeded())
			Sk1 = Sk1Asset.Class;
}

void UPP_ArachBossAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPP_ArachBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPP_ArachBossAnim::AnimNotify_Skill1()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Boss sk1")));

	APP_ArachBoss* Owner = Cast<APP_ArachBoss>(TryGetPawnOwner());
	/* m_Target 퍼블릭아님, 수정필요
	if (Owner)
	{
		if (Owner->m_Target)
		{
			if (Sk1)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				FVector Loc = FVector(Owner->m_Target->GetActorLocation().X, Owner->m_Target->GetActorLocation().Y, Owner->m_Target->GetActorLocation().Z - 50.f);

				APP_ArachBossSkill1* pSkill = GetWorld()->SpawnActor<APP_ArachBossSkill1>(Sk1, Loc,
					Owner->m_Target->GetActorRotation(), param);
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("none Sk1")));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("none target")));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("none owner")));
		*/
}

void UPP_ArachBossAnim::AnimNotify_Skill1End()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Boss sk1 end")));
	MyAnimType = AnimType::Idle;

	
}