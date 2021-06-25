#pragma once

#include "PP_GameInstance.h"
#include "PP_PlayerAIController.h"
#include "PP_ElfSwordMasterAIController.generated.h"

/**
 * 
 */
UCLASS()
class ELFPALADINTEMPLATE_API APP_ElfSwordMasterAIController : public APP_PlayerAIController
{
	GENERATED_BODY()
public:
	APP_ElfSwordMasterAIController();

	void SetAIType(PlayerAIType type);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	PlayerAIType MyAIType;

};
