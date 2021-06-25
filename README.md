# Unreal4_RPG_portfolio
1. 플레이어

3가지 캐릭터의 포인터를 갖고 있는 컨트롤러 클래스를 통해서 사용자의 입력을 받고 캐릭터들을 조작합니다.
조작 방식은 화면내의 액터들을 클릭하거나 UI버튼을 누르는 방식으로 이동, 자동전투, 방어, 아이템 사용, 각 캐릭터들의 스킬을 사용등의 동작을 수행합니다.
각 직업의 캐릭터들은 PP_Player클래스를 상속 받고, 각 캐릭터 특성에 따른 레벨, 스킬사용등의 함수를 구현하였고
플레이어의 입력 또는 자동전투 방식에 따라 애니메이션 인스턴스를 상속받은 각 _Anim클래스의 함수들을 호출하여 애니메이션 상태를 전환 하고,
애니메이션의 노티파이 함수에서 각 직업의 스킬을 호출하는 방식으로 공격과 스킬, 방어등을 사용합니다.
각 캐릭터들의 스킬은 다음과 같습니다

기사 - 자신을 주변 몬스터들의 타겟으로, 방어자세 후 받은 대미지만큼 반격, 공격업(지속적으로 체력 감소) 버프, 4연속 단일공격, 범위공격 후 HP흡수

힐러 - 단일 힐, 파티 힐, 단일 상태이상제거, 단일 부활, 전체 부활 및 HP,MP회복

마법사 - 단일 개체 공격 후 스플래시, 장판공격, 단일 마비, 범위 공격, 단일공격 후 MP흡수


1.2 코드

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
	//기타 조작이 없는 액터를 눌렀을 시 초기화
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

PP_ElfSwordMaster, PP_ElfSwordMasterAnim 기사

PP_HighPriestess, PP_HighPriestessAnim 힐러

PP_Player 각 캐릭터들의 부모 클래스 (언리얼의 캐릭터 클래스 상속)

PP_PlayerController 전체 캐릭터를 조작하기 위한 입력을 받는 클래스 (언리얼의 플레이어컨트롤러 상속)

PP_GameInstance 캐릭터정보와 아이템정보를 관리하는 클래스 (언리얼의 게임인스턴스를 상속)

PP_Wizard, PP_WizardAnim 마법사





2. 몬스터
PP_Monster클래스를 상속받은 11가지의 몬스터들이 있으며 각 몬스터들의 특징은 아래와 같습니다.
고블린 - 탐지(플레이어 캐릭터를 탐색) 후 근거리 공격

오크서포터 - 탐지 후 사거리 내의 원거리 공격 및 근처 HP비율이 낮은 몬스터에게 힐

오크 - 탐지 후 마비공격, 전체마비 등의 스킬사용

스콜피 - 탐지가 아닌 자신을 공격한 플레이어를 근거리 공격, 타겟이 없을 시 근처를 랜덤하게 이동

아라크네 - 고정된 자리에서 거리에따른 근,원거리 공격과 장판스킬, 독스킬, 몬스터를 소환

붐스파이더 - 고블린과 같은 동작을 하나 사망 시 주변에 대미지

텐타클몬스터 - 붐스파이더와 같은 동작을 하나 기본공격에 확률적 상태이상 추가

만드레이크 - 단순히 시간마다 주변 캐릭터를 마비상태이상으로 만들고, TakeDamage를 오버라이드해 대미지를 준 캐릭터에 거리가 멀면 대미지를 0으로 처리

서큐버스 - 3방향 원거리 공격과 전체 상태이상, 범위 스킬, 가장 먼 거리의 플레이어 캐릭터 뒤로 순간이동등의 스킬 사용

사탄 - 5방향 원거리 공격과 다양한 범위스킬(자신위치에 장판, 특정 캐릭터 주변에 대미지, 매테오) 스킬 사용

라보스 - 고정된 자리에서 다양한 범위스킬(바둑판식 장판, 4분면매태오, 전체공격 후 흡혈)과 상태이상, 즉사공격 스킬 사용


2.2 코드

2.1 탐지 방식1

	if (!m_Target)
		{	
			TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), DetectRange, GetWorld(), this, ECC_GameTraceChannel4, false);
			for (auto& hitted : temp)
			{
				APP_Player* temhit = Cast< APP_Player>(hitted.GetActor());
				if (temhit)
				{
					if (temhit->GetPlayerInfo()->Hp > 0)
					{
						m_Target = Cast<AActor>(hitted.GetActor());
						break;
					}
				}
			}
		}
		if (m_Target)
		{
			float dist = FVector::Distance(m_Target->GetActorLocation(), GetActorLocation());
			if (dist > AttackRange)
			{
				if (MyAnim)
					MyAnim->SetAnimState(AnimType::Move);
				UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
			}
			else
			{
				GetController()->StopMovement();
				if (MyAnim)
				{
					if (MyAnim->GetAnimState() != AnimType::Attack1)
					{
						FVector	Dir = m_Target->GetActorLocation() - GetActorLocation();
						Dir.Normalize();
						SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
						MyAnim->SetAnimState(AnimType::Attack1);
					}
				}
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none target")));
			GetController()->StopMovement();
			if (MyAnim)
				MyAnim->SetAnimState(AnimType::Idle);
		}
	
	
2.1 탐지 방식2


	
2.3 관련 클래스

PP_ArachnidBoss, PP_ArachnidBossAnim 레벨2의 보스

PP_BoomSpider, PP_BoomspiderAnim 레벨2보스의 소환수

PP_Goblin, PP_GoblinAnim 레벨1의 일반 몬스터

PP_LastBoss, PP_LastBossAnim 레벨4의 최종 보스

PP_Mandrake, PP_MandrakeAnim 레벨3의 퍼즐 몬스터

PP_Monster 언리얼의 캐릭터 클래스를 상속 받은 모든 몬스터들의 기본이 되는 클래스

PP_MonsterSpawnPotin 시간에 따라 몬스터를 스폰시키는 클래스

PP_Orc, PP_OrcAnim 레벨2의 보스

PP_OrcSupporter,PP_OrcSupporterAnim 레벨2의 보스

PP_Satan, PP_SatanAnim 레벨3의 보스

PP_Scorpi, PP_ScorpiAnim 레벨2의 몬스터

PP_Succubus, PP_SuccubusAnim 레벨3의 보스

PP_TectacleMonster, PP_TentacleMonsterAnim 레벨3의 몬스터



3. 스킬



3. UI 및 시스템



4. 기타

