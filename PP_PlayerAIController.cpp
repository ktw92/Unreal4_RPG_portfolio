// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_PlayerAIController.h"

APP_PlayerAIController::APP_PlayerAIController()
{
	/*
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	TreeObj(TEXT("BehaviorTree'/Game/Monster/BTRampageAI.BTRampageAI'"));

	if (TreeObj.Succeeded())
		m_AITree = TreeObj.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardObj(TEXT("BlackboardData'/Game/Monster/BBRampage.BBRampage'"));

	if (BlackboardObj.Succeeded())
		m_AIBlackBoard = BlackboardObj.Object;
		*/
}


void APP_PlayerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("ai ride on the character")));
	/*
	// 행동트리를 지정한다.
	if (UseBlackboard(m_AIBlackBoard, Blackboard))
	{
		// Blackboard에 정보를 넣어둔다.
		ARampage* pRampage = Cast<ARampage>(InPawn);

		Blackboard->SetValueAsFloat(TEXT("TraceRange"), pRampage->GetDetectRange());
		Blackboard->SetValueAsFloat(TEXT("AttackRange"), pRampage->GetAttackDistance());

		if (!RunBehaviorTree(m_AITree))
			LOG(TEXT("Rampage BehaviorTree Error!!"));
	}
	*/
}

void APP_PlayerAIController::OnUnPossess()
{
	Super::OnUnPossess();

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("ai unride on the character")));
}
