// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_GoblinAIController.h"

APP_GoblinAIController::APP_GoblinAIController()
{
	/*
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	TreeObj(TEXT("BehaviorTree'/Game/Monster/BTSevarog.BTSevarog'"));

	if (TreeObj.Succeeded())
		m_AITree = TreeObj.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardObj(TEXT("BlackboardData'/Game/Monster/BBSevarog.BBSevarog'"));

	if (BlackboardObj.Succeeded())
		m_AIBlackBoard = BlackboardObj.Object;
		*/
}


void APP_GoblinAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	/*
	// 행동트리를 지정한다.
	if (UseBlackboard(m_AIBlackBoard, Blackboard))
	{
		// Blackboard에 정보를 넣어둔다.
		ASevarog* pSevarog = Cast<ASevarog>(InPawn);

		Blackboard->SetValueAsFloat(TEXT("TraceRange"), pSevarog->GetDetectRange());
		Blackboard->SetValueAsFloat(TEXT("AttackRange"), pSevarog->GetAttackDistance());
		Blackboard->SetValueAsFloat(TEXT("AttackRange2"), pSevarog->GetAttackDistance2());
		Blackboard->SetValueAsBool(TEXT("SkillEnable"), false);
		Blackboard->SetValueAsEnum(TEXT("SkillType"), (uint8)ESevarogSkillType::None);

		if (!RunBehaviorTree(m_AITree))
			LOG(TEXT("Sevarog BehaviorTree Error!!"));
	}
	*/
}

void APP_GoblinAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
