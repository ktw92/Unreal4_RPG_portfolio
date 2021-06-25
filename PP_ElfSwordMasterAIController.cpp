// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ElfSwordMasterAIController.h"
#include "PP_ElfSwordMaster.h"


APP_ElfSwordMasterAIController::APP_ElfSwordMasterAIController()
{
	//비헤이비어트리
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	TreeObj(TEXT("BehaviorTree'/Game/0_PP/0_Player/ElfSwordMaster/BT_PPElfSwordMaster.BT_PPElfSwordMaster'"));
	if (TreeObj.Succeeded())
		AITree = TreeObj.Object;
	//블랙보드
	static ConstructorHelpers::FObjectFinder<UBlackboardData>	BlackboardObj(TEXT("BlackboardData'/Game/0_PP/0_Player/ElfSwordMaster/BB_PPElfSwordMaster.BB_PPElfSwordMaster'"));
	if (BlackboardObj.Succeeded())
		AIBlackBoard = BlackboardObj.Object;		

	MyAIType = PlayerAIType::NormalMove;
}


void APP_ElfSwordMasterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	
	// 행동트리를 지정한다.
	if (UseBlackboard(AIBlackBoard, Blackboard))
	{
		// Blackboard에 정보를 넣어둔다.
		//ARampage* pRampage = Cast<ARampage>(InPawn);
		//Blackboard->SetValueAsFloat(TEXT("TraceRange"), pRampage->GetDetectRange());
		//Blackboard->SetValueAsFloat(TEXT("AttackRange"), pRampage->GetAttackDistance());

		APP_ElfSwordMaster* me = Cast<APP_ElfSwordMaster>(InPawn);
		if (me)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("elfsword cast ok")));
			UObject* temp = Cast<UObject>(me);
			if (temp)
			{
				Blackboard->SetValueAsObject("me", temp);
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("elf obj cast ok")));
			}
		}

		
	}

}

void APP_ElfSwordMasterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void APP_ElfSwordMasterAIController::SetAIType(PlayerAIType type)
{
	MyAIType = type;
	Blackboard->SetValueAsEnum("PlayerAi", (uint8)(type));
}