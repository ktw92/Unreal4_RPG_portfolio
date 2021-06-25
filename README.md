# Unreal4_RPG_portfolio

1. 플레이어


1.2 핵심 코드

1.2.1 마우스 입력으로 동작 하는 함수 APP_PlayerController::MovePicking 일부분
마우스로 클릭한 대상에 따라서 알맞은 처리를 합니다


	if (bHit)
	{
		
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

1.2.2 플레이어 캐릭터의 틱 APP_Player::Tick 일부분

3가지 캐릭터의 공통적인 상태이상이나 자동전투에 따른 행동을 처리합니다

	//시체면 타겟제거 및 초기화
	if (TargetedMonster)
	{
		APP_Monster* temp_monp = Cast<APP_Monster>(TargetedMonster);
		if (temp_monp)
		{
			if (temp_monp->GetStatus()->Hp <= 0)
			{
				TargetFree();
				SimpleStop();
				ChangeAnimState(AnimType::Idle);
			}
		}
	}

	//스킬쿨타임복구
	for (int i = 0; i < 5; i++)
	{
		if (PlayerInfo.Skill_cooltimeAcc[i] < PlayerInfo.Skill_cooltime[i])
		{
			PlayerInfo.Skill_cooltimeAcc[i] += DeltaTime;
		}
		APP_PlayerController* tempcon = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (tempcon)
		{
			if (this == tempcon->GetControlCharacter())
			{
				UPP_MainWidgetFrame* temp_frame = tempcon->GetMainWidget();
				temp_frame->SetSkillCoolTime(i, 1 - (PlayerInfo.Skill_cooltimeAcc[i] / PlayerInfo.Skill_cooltime[i]));
			}
		}
	}

	if (PlayerInfo.Debuff[0] == true)//마비상태면
	{
		PlayerInfo.Paralysis_Acc += DeltaTime;
		if (PlayerInfo.Paralysis_Acc >= ParalysisTime)
		{
			PlayerInfo.Paralysis_Acc = 0;
			PlayerInfo.Debuff[0] = false;
			SetIdle();
			if (MyPortraitWidget)
				MyPortraitWidget->ReSetDenuff(0);
		}
	}

	if (PlayerInfo.Debuff[1]) //중독이면
	{
		PlayerInfo.Poison_Acc += DeltaTime;
		float temp_damage = DeltaTime * 0.05f * PlayerInfo.Max_hp;
		HpDown(temp_damage);
		if (PoisonTime < PlayerInfo.Poison_Acc)
		{
			PlayerInfo.Poison_Acc = 0;
			PlayerInfo.Debuff[1] = false;
			if (MyPortraitWidget)
				MyPortraitWidget->ReSetDenuff(1);
		}
		if ((int)(PlayerInfo.Poison_Acc + 0.1f) > (int)(PlayerInfo.Poison_Acc))//대략초당 한번 이펙트 발생
		{
			if (Poison)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<APP_EffectBase>(Poison, GetActorLocation() + FVector(0,0,120),
					FRotator::ZeroRotator, param);
			}
		}
	}

	//이동중일때
	if (isMoving && !TargetedMonster)
	{	
		if(!GetController()->IsFollowingAPath()) //멈췄을때
		{
			isMoving = false;
			ChangeAnimState(AnimType::Idle);

			GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
			
			FVector endp = FVector(GetActorLocation().X + GetMesh()->GetForwardVector().X*1000, GetActorLocation().Y + GetMesh()->GetForwardVector().Y*1000, GetActorLocation().Z);

			FHitResult temp;
			bool bSweep = GetWorld()->LineTraceSingleByChannel(temp, GetActorLocation(), endp, ECollisionChannel::ECC_GameTraceChannel3);

			GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));
		}
		else
		{
			//목표로 회전
			FVector Dir = MoveGoal - GetActorLocation();
			Dir.Normalize();
			GetMesh()->SetWorldRotation(FRotator(0.f, Dir.Rotation().Yaw - 90.f, 0.f));
		}
	
	}
	//자동전투중
	else if (isAutoBattle)
	{
		//타겟이 있으면 거리 측정 후 공격 또는 추격
		if (TargetedMonster)
		{
			//일단 이동
			if ((PlayerInfo.AnimState != AnimType::Attack1))
			{
				AActor* tempm1 = TargetedMonster;
				SimpleMove(TargetedMonster->GetActorLocation());
				TargetedMonster = tempm1;
			}
		
			if (PlayerInfo.AnimState != AnimType::Attack1)
			{
					float dist = FVector::Dist(GetActorLocation(), TargetedMonster->GetActorLocation());
					if (dist <= PlayerInfo.Attack_range) //공격거리에 있으면 공격
					{
						FVector dir = TargetedMonster->GetActorLocation() - GetActorLocation();
						dir.Normalize();
						GetMesh()->SetWorldRotation(FRotator(0.f, dir.Rotation().Yaw - 90.f, 0.f));
						SimpleStop();
						ChangeAnimState(AnimType::Attack1);
						
					}
					else //공격중거리에 없으면 이동
					{
						AActor* tempm = TargetedMonster;
						SimpleMove(TargetedMonster->GetActorLocation());
						TargetedMonster = tempm;
						
					}
			}
			else //(PlayerInfo.AnimState == AnimType::Attack1)
			{
				float dist = FVector::Dist(GetActorLocation(), TargetedMonster->GetActorLocation());
				if (dist >= PlayerInfo.Attack_range) //공격거리에 벗어나 있으면 다시 이동
				{
					AActor* tempm2 = TargetedMonster;
					SimpleMove(TargetedMonster->GetActorLocation());
					TargetedMonster = tempm2;
				}
			}
			FVector Dir = TargetedMonster->GetActorLocation() - GetActorLocation();
			Dir.Normalize();
			GetMesh()->SetWorldRotation(FRotator(0.f, Dir.Rotation().Yaw - 90.f, 0.f));
		}
		else //타겟 없으면 탐지
		{
			TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), PlayerInfo.Attack_range * 20, GetWorld(), this, ECC_GameTraceChannel3, true);
			float mindist = 123456789;
			for (auto& hitted : temp)
			{
				APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
				if (HittedMonster)
				{
					//시체는 안건드림
					if (HittedMonster->GetStatus()->Hp <= 0)
					{
						ChangeAnimState(AnimType::Idle);
					}
					else
					{
						//가장 가까운 개체를 타겟으로
						float dist = FVector::Dist(GetActorLocation(), HittedMonster->GetActorLocation());
						if (dist < mindist)
						{
							mindist = dist;
							TargetedMonster = HittedMonster;
						}
					}			
				}
			}
		}
	}
	//몬스터클릭해서 자동공격인 상태
	else if(TargetedMonster)
	{
		//몬스터 죽었는지 확인
		APP_Monster* temp_mon = Cast< APP_Monster>(TargetedMonster);
		if (temp_mon)
		{
			if (temp_mon->GetStatus()->Hp <= 0)
			{
				SimpleStop();
				TargetFree();
			}
		}
		
		if (PlayerInfo.AnimState != AnimType::Attack1)
		{
			float dist = FVector::Dist(GetActorLocation(), TargetedMonster->GetActorLocation());
			if (dist <= PlayerInfo.Attack_range) //공격거리에 있으면
			{
				FVector dir = TargetedMonster->GetActorLocation() - GetActorLocation();
				dir.Normalize();
				GetMesh()->SetWorldRotation(FRotator(0.f, dir.Rotation().Yaw - 90.f, 0.f));
				SimpleStop();
				ChangeAnimState(AnimType::Attack1);
			}
			else //거리에 없으면 계속이동
			{
				AActor* tempm = TargetedMonster;
				SimpleMove(TargetedMonster->GetActorLocation());
				TargetedMonster = tempm;
			}
		}
		else //공격중에 거리에서 멀어지면 이동
		{
			float dist = FVector::Dist(GetActorLocation(), TargetedMonster->GetActorLocation());
			if (!(dist <= PlayerInfo.Attack_range)) //공격거리에 있으면
			{
				AActor* tempm = TargetedMonster;
				SimpleMove(TargetedMonster->GetActorLocation());
				TargetedMonster = tempm;
			}
		}

		FVector Dir = TargetedMonster->GetActorLocation() - GetActorLocation();
		Dir.Normalize();
		GetMesh()->SetWorldRotation(FRotator(0.f, Dir.Rotation().Yaw - 90.f, 0.f));
	}
	else if (isAssemble) //다 끝내고 집합중
	{
	isAssemble;
	}
	else
	{
		SimpleStop();
	}

1.3 관련 클래스

PP_ElfSwordMaster 기사

PP_ElfSwordMasterAnim 기사 애니메이션

PP_HighPriestess 힐러

PP_HighPriestessAnim 힐러 애니메이션

PP_Player 각 캐릭터들의 부모 클래스 (언리얼의 캐릭터 클래스 상속)

PP_PlayerController 전체 캐릭터를 조작하기 위한 입력을 받는 클래스 (언리얼의 플레이어컨트롤러 상속)

PP_GameInstance 캐릭터정보와 아이템정보를 관리하는 클래스 (언리얼의 게임인스턴스를 상속)

PP_Wizard 마법사

PP_WizardAnim 마법사 애니메이션


2. 몬스터

3. 스킬

3. UI 및 시스템

4. 기타

