# Unreal4_RPG_portfolio

1. 플레이어


1.2 관련 클래스

PP_ElfSwordMaster 기사

PP_ElfSwordMasterAnim 기사 애니메이션

PP_HighPriestess 힐러

PP_HighPriestessAnim 힐러 애니메이션

PP_Player 각 캐릭터들의 부모 클래스 (언리얼의 캐릭터 클래스 상속)

PP_PlayerController 전체 캐릭터를 조작하기 위한 입력을 받는 클래스 (언리얼의 플레이어컨트롤러 상속)

PP_GameInstance 캐릭터정보와 아이템정보를 관리하는 클래스 (언리얼의 게임인스턴스를 상속)

PP_Wizard 마법사

PP_WizardAnim 마법사 애니메이션


1.3 핵심 코드
1.3.1 마우스 킬릭스 동작 하는 함수 APP_PlayerController::MovePicking 일부분


	if (bHit)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f, %f, %f"), result.Location.X, result.Location.Y, result.Location.Z));
		if (ControlCharacter && !ControlCharacter->GetPlayerInfo()->Debuff[0])
		{
			APP_Player* isPlayer = Cast<APP_Player>(result.Actor);
			APP_Monster* isMon = Cast<APP_Monster>(result.Actor);
			if (isMon && !ControlCharacter->GetPlayerInfo()->isDead) //몬스터클릭
			{
				AActor* temptarget = Cast<AActor>(result.Actor);
				if (temptarget)
				{
					if (isTargettingSkill) //타겟스킬 대기중이면
					{
						ControlCharacter->SetSKillTarget(temptarget, SkillNum);
						ControlCharacter->TargetFree();
						//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill target on")));
					}
					else
						ControlCharacter->SetTargetMonster(temptarget);
				}
			}
			else if (isPlayer) //플레이어클릭
			{ 
				if (isTargetItem  && !ControlCharacter->GetPlayerInfo()->isDead && !ControlCharacter->GetPlayerInfo()->Debuff[0]) //소비템 사용대기중이면
				{
					ControlCharacter->UsingItem(ItemType, isPlayer);
					SetUsingItemOff();
				}
				else if (isTargettingSkill && isPlayerTarget && !ControlCharacter->GetPlayerInfo()->isDead && !ControlCharacter->GetPlayerInfo()->Debuff[0])
				{
					ControlCharacter->SetSKillTarget(isPlayer, SkillNum);
					ControlCharacter->TargetFree();
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill target on")));
				}
				else //컨트롤캐릭전환
				{
				
					ControlCharacter = isPlayer;
					ControlCharacter->SetAutoBattleOnOff(false, false);
					SetViewTargetWithBlend(Cast<AActor>(ControlCharacter), 0.7f, EViewTargetBlendFunction::VTBlend_EaseIn, 15, true);
					//스킬아이콘 전환
					if (MyMainWidget)
						MyMainWidget->ChangeSkill_Icon(ControlCharacter->GetPlayerInfo()->PortraitNum);
					//장비창 전환
					ChangeEquip();
					//MyMainWidget->GetEquipWidget()->SetImageAndVisible(ControlCharacter->GetPlayerInfo()->PortraitNum - 1, (int)(ControlCharacter->GetPlayerInfo()->MyItem), true);
				}
			}
			else
			{
				APP_ShopNPC* isShop = Cast<APP_ShopNPC>(result.Actor);
				if(isShop)//상점클릭
				{
					float dist = FVector::Distance(isShop->GetActorLocation(), ControlCharacter->GetActorLocation());
					if(dist < 800)
					{
						isShop->SetTrade(true);
						MyMainWidget->ShopOpen();
					}
				}

				if(!ControlCharacter->GetSkillTarget() && !ControlCharacter->GetPlayerInfo()->isDead
					&& !ControlCharacter->GetPlayerInfo()->Debuff[0] && !ControlCharacter->IsSkill())//스킬중이면 이동불가
					ControlCharacter->SimpleMove(FVector(result.Location.X, result.Location.Y, result.Location.Z));
			}
		}
	//다른거 눌렀을 시 초기화
	SkillTargetFree();
	SetUsingItemOff();


2. 몬스터

3. 스킬

3. UI 및 시스템

4. 기타

