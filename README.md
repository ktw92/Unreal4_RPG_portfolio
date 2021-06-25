# Unreal4_RPG_portfolio

엔진버전 4.26

개발기간 4주

싱글RPG 개인프로젝트

[![Video Label](https://i.ytimg.com/an_webp/Tv0JURa6t2s/mqdefault_6s.webp?du=3000&sqp=CPfq1oYG&rs=AOn4CLCJgDw7KFfydDxXmHahP3CO3JiwUg)](https://youtu.be/Tv0JURa6t2s)

https://youtu.be/Tv0JURa6t2s
<br /> 
## 1. 플레이어

3가지 캐릭터의 포인터를 갖고 있는 컨트롤러 클래스를 통해서 사용자의 입력을 받고 캐릭터들을 조작합니다.
조작 방식은 화면내의 액터들을 클릭하거나 UI버튼을 누르는 방식으로 이동, 자동전투, 방어, 아이템 사용, 각 캐릭터들의 스킬을 사용등의 동작을 수행합니다.
각 직업의 캐릭터들은 PP_Player클래스를 상속 받고, 각 캐릭터 특성에 따른 레벨, 스킬사용등의 함수를 구현하였고
플레이어의 입력 또는 자동전투 방식에 따라 애니메이션 인스턴스를 상속받은 각 _Anim클래스의 함수들을 호출하여 애니메이션 상태를 전환 하고,
애니메이션의 노티파이 함수에서 각 직업의 스킬을 호출하는 방식으로 공격과 스킬, 방어등을 사용합니다.

각 캐릭터들의 스킬은 다음과 같습니다(일부 스킬의 GIF)

![knight12](https://user-images.githubusercontent.com/76766673/123456380-bdb3b980-d61d-11eb-8daa-26fbde157233.gif)

기사 - 자신을 주변 몬스터들의 타겟으로, 방어자세 후 받은 대미지만큼 반격, 공격업(지속적으로 체력 감소) 버프, 4연속 단일공격, 범위공격 후 HP흡수

![heal12](https://user-images.githubusercontent.com/76766673/123456386-bf7d7d00-d61d-11eb-81f9-497c55ddbab4.gif)

힐러 - 단일 힐, 파티 힐, 단일 상태이상제거, 단일 부활, 전체 부활 및 HP,MP회복

![wizard24](https://user-images.githubusercontent.com/76766673/123457202-a4f7d380-d61e-11eb-8456-9ecae23a1d77.gif)

마법사 - 단일 개체 공격 후 스플래시, 장판공격, 단일 마비, 범위 공격, 단일공격 후 MP흡수

<br /> 
<br /> 


# 1.2 코드

# 1.2.1 마우스 입력으로 동작 하는 함수 APP_PlayerController::MovePicking
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
	
<br /> 

# 1.2.2 플레이어 캐릭터의 틱 APP_Player::Tick

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

<br /> 

# 1.3 관련 클래스

PP_ElfSwordMaster, PP_ElfSwordMasterAnim 기사

PP_HighPriestess, PP_HighPriestessAnim 힐러

PP_Player 각 캐릭터들의 부모 클래스 (언리얼의 캐릭터 클래스 상속)

PP_PlayerController 전체 캐릭터를 조작하기 위한 입력을 받는 클래스 (언리얼의 플레이어컨트롤러 상속)

PP_GameInstance 캐릭터정보와 아이템정보를 관리하는 클래스 (언리얼의 게임인스턴스를 상속)

PP_Wizard, PP_WizardAnim 마법사


<br /> 
<br /> 
<br /> 


## 2. 몬스터
PP_Monster클래스를 상속받은 11가지의 몬스터들이 있으며 각 몬스터들의 특징은 아래와 같습니다.

고블린 - 탐지(플레이어 캐릭터를 탐색) 후 근거리 공격

오크서포터 - 탐지 후 사거리 내의 원거리 공격 및 근처 HP비율이 낮은 몬스터에게 힐

![boss1](https://user-images.githubusercontent.com/76766673/123457237-acb77800-d61e-11eb-9fec-8c03c111b8c2.gif)

오크 - 탐지 후 마비공격, 전체마비 등의 스킬사용

스콜피 - 탐지가 아닌 자신을 공격한 플레이어를 근거리 공격, 타겟이 없을 시 근처를 랜덤하게 이동

![boss2](https://user-images.githubusercontent.com/76766673/123457246-ae813b80-d61e-11eb-9f54-b54e0b77e9aa.gif)

아라크네 - 고정된 자리에서 거리에따른 근,원거리 공격과 장판스킬, 독스킬, 몬스터를 소환

붐스파이더 - 고블린과 같은 동작을 하나 사망 시 주변에 대미지

텐타클몬스터 - 붐스파이더와 같은 동작을 하나 기본공격에 확률적 상태이상 추가

만드레이크 - 단순히 시간마다 주변 캐릭터를 마비상태이상으로 만들고, TakeDamage를 오버라이드해 대미지를 준 캐릭터에 거리가 멀면 대미지를 0으로 처리

![boss3](https://user-images.githubusercontent.com/76766673/123457395-d7093580-d61e-11eb-8deb-ba593e191589.gif)

서큐버스 - 3방향 원거리 공격과 전체 상태이상, 범위 스킬, 가장 먼 거리의 플레이어 캐릭터 뒤로 순간이동등의 스킬 사용

사탄 - 5방향 원거리 공격과 다양한 범위스킬(자신위치에 장판, 특정 캐릭터 주변에 대미지, 매테오) 스킬 사용

![boss4](https://user-images.githubusercontent.com/76766673/123457441-e1c3ca80-d61e-11eb-95da-2a28d7d81b79.gif)

라보스 - 고정된 자리에서 다양한 범위스킬(바둑판식 장판, 4분면매태오, 전체공격 후 흡혈)과 상태이상, 즉사공격 스킬 사용

<br /> 
<br /> 

# 2.2 코드

# 2.2.1 탐지 방식1

World->Sweep~ 을 사용한 방식 (코드 중복을 제거하기위해 자체 함수고 함수안에서 호출 

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
	
<br /> 

# 2.2.2 탐지 방식2

3개의 플레이어 캐릭터만 가져와 거리 비교로 측정

	if (!m_Target)
	{
		//가장 가까운 캐릭터를 타겟으로
		APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		Idle();
		if (temp_controller)
		{
			TArray<APP_Player*>* players = temp_controller->GetMyParty();
			float mindist = 123456789;
			if (players)
			{
				for (auto& player : *players)
				{
					if (player)
					{
						float temp_dist = FVector::Distance(GetActorLocation(), player->GetActorLocation());
						if (temp_dist < mindist)
						{
							//시체는 안건드림
							if (player->GetPlayerInfo()->Hp <= 0)
							{
								//HittedMonster->ChangeAnimState(AnimType::Death);
								continue;
							}
							mindist = temp_dist;
							m_Target = player;
						}
					}
				}
			}
		}
	}

<br /> 

# 2.2.3 스킬 쿨타임

Tick함수 내부에서 다음과 같이 스킬쿨타임을 체크하고 스킬 상태일 시 다른동작을 하지않게 return으로 종료 합니다

	if (isSkillCast)
	{
		GetController()->StopMovement();
		return;
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			SkillCoolTime[i] += DeltaTime;
			if (SkillCoolTime[i] >= MonsterStatus.Skill_cooltime[i])
			{
				isSkillCast = true;
				SkillCoolTime[i] = 0;
				switch (i)
				{
				default:
					break;
				case 0: Skill1(); break;
				case 1: Skill2(); break;
				case 2: Skill3(); break;
				case 3: Skill4(); break;
				case 4: Skill5(); break;
				}
				GetController()->StopMovement();
				return;
			}
		}
	}
<br /> 

# 2.2.4 랜덤한 플레이어에 주변에 1차공격 후 2차 공격하는 스킬

![8way](https://user-images.githubusercontent.com/76766673/123457541-fdc76c00-d61e-11eb-8d7d-270aa3959f68.gif)


	if (IsFirst)
			{
				TArray<APP_Player*>* players = temp_controller->GetMyParty();
				FRandomStream Rs;
				Rs.GenerateNewSeed();
				int tempint = Rs.FRandRange(0, 2.99f);
				APP_Player* Skill2Target = nullptr;
				if ((*players).Num() <= 3)
					Skill2Target = (*players)[tempint];
				if (Skill2Target)
				{
					for (int i = 0; i < 8; i++)
					{
						Skill2Target->GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + i * 45.f, 0.f));
						APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill2Effect, Skill2Target->GetActorLocation() + 		Skill2Target->GetMesh()->GetForwardVector() * 400,
							FRotator::ZeroRotator, param);
						temp->SetStatus(1.1f, 0, MonsterStatus.Attack[0] * 1.5f, 225, FVector(0, 0, 0), ECC_GameTraceChannel4, this);
						GetMesh()->SetRelativeRotation(FRotator(0.f, Skill2Target->GetMesh()->GetRelativeRotation().Yaw - i * 45.f, 0.f));
					}
				}
				Skill2Targetpos = Skill2Target->GetActorLocation();
			}
			else
			{
				APP_ProjectileBoom* temp = GetWorld()->SpawnActor<APP_ProjectileBoom>(Skill2Effect, Skill2Targetpos,
						FRotator::ZeroRotator, param);
			}

<br /> 
<br /> 

# 2.3 관련 클래스

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

<br /> 
<br /> 
<br /> 

## 3. 스킬
스킬의 형식에따라 클래스들을 만들었으며 블루프린트로 만든 후 각 스킬들의 특성에 맞게 사운드, 비주얼효과(파티클,나이아가라) 리소스를 넣어주었습니다.
플레이어, 몬스터 공용으로 사용할 수 있게 채널을 따로 갖고 그 채널에 따라 알맞은 동작을 하도록 구현 했습니다.
초기 기획 외에 마지막에 추가된 스킬들이 있어 코드 중복을 줄이지 못한점은 아쉬움이 남는 점 입니다.

<br /> 

# 3.2 코드

# 3.2.1 기본 원거리 공격

![normal](https://user-images.githubusercontent.com/76766673/123457656-1899e080-d61f-11eb-9276-bdba152dadba.gif)

할당된 채널에 따라 플레이어,몬스터 공동 사용이 가능하게 구현 했습니다.

	{
		//충돌감지 후 터짐
		FHitResult temp = DetectSphere(GetActorLocation(), Radius, GetWorld(), Owner, Type);
		if (Type == ECC_GameTraceChannel3)//플레이어의 공격
		{
			APP_Monster* isMon = Cast<APP_Monster>(temp.Actor);
			if (isMon)
			{
				if (isMon->GetStatus()->Hp > 0) //시체가 아니면
				{
					FDamageEvent	DmgEvent;
					UGameplayStatics::GetPlayerController(GetWorld(), 0);
					isMon->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					isEnd = true;
				}
			}
		}
		else //(Type == ECC_GameTraceChannel4);
		{
			APP_Player* isP = Cast<APP_Player>(temp.Actor);
			if (isP)
			{
				//대미지 주고
				FDamageEvent	DmgEvent;
				UGameplayStatics::GetPlayerController(GetWorld(), 0);
				isP->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
				isEnd = true;
			}
		}

		//이동
		FVector next = GetActorLocation() + Forward * DeltaTime * Speed;
		SetActorLocation(next);
	}

<br /> 

# 3.2.2 이동하는 시한폭탄 PP_ProjectileBoom::Tick

![boom](https://user-images.githubusercontent.com/76766673/123457746-323b2800-d61f-11eb-941c-e5f094fad1b2.gif)

스킬을 사용하는 캐릭터가 클래스를 월드에 생성한 후 페러미터 할당 함수로 변수들의 값(시간,대미지 등)을 조절하면 그에 따라 동작하도록 구현 했습니다.

	if (isEnd)
	{
		endtime += DeltaTime;
		if (endtime >= 1.5f)
			Destroy();
		return;
	}
	else if (isStart)
	{
		//시한폭탄
		LifeAcc += DeltaTime;
		if (LifeAcc >= LifeTime)
		{
			isEnd = true;
			isStart = false;
		}


		//터짐
		if (isEnd)
		{
			if (End_Sound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), End_Sound, GetActorLocation());

			My_Particle->SetActive(false);
			My_Particle->SetWorldScale3D(FVector(0, 0, 0));
			My_Niagara->SetActive(false);
			My_Niagara->SetWorldScale3D(FVector(0, 0, 0));
			if (My_HittedNiagara)
			{
				My_HittedNiagara->ActivateSystem();
				My_HittedNiagara->AttachTo(My_RootScene);
				My_HittedNiagara->Activate(true);
				My_HittedNiagara->SetActive(true);
			}
			if (My_HittedParticle)
			{
				My_HittedParticle->ActivateSystem();
				My_HittedParticle->AttachTo(My_RootScene);
				My_HittedParticle->Activate(true);
				My_HittedParticle->SetActive(true);
			}

			TArray<FHitResult> temp = SphereMulti(GetActorLocation(), Radius, GetWorld(), Owner, Type);
			for (auto& hitted : temp)
			{
				if (Type == ECC_GameTraceChannel3)//플레이어의 공격
				{
					APP_Monster* isMon = Cast<APP_Monster>(hitted.Actor);
					if (isMon)
					{
						if (isMon->GetStatus()->Hp > 0) //시체가 아니면
						{
							FDamageEvent	DmgEvent;
							UGameplayStatics::GetPlayerController(GetWorld(), 0);
							isMon->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
						}
					}
				}
				else //(Type == ECC_GameTraceChannel4);
				{
					APP_Player* isP = Cast<APP_Player>(hitted.Actor);
					if (isP)
					{
						//대미지 주고
						FDamageEvent	DmgEvent;
						UGameplayStatics::GetPlayerController(GetWorld(), 0);
						isP->TakeDamage(Damage, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), Owner);
					}
				}
			}
		}

		//이동
		FVector next = GetActorLocation() + Forward * DeltaTime * Speed;
		SetActorLocation(next);
	}

<br /> 

# 3.3 관련 클래스

PP_CircleMoveAttack 특정 캐릭터 주변을 회전하며 닿는 적에게 대미지를 줌

PP_DotDamageSkill 장판기술로 범위 내의 캐릭터에게 대미지를 줌

PP_DotHealSkill 장판기술로 범위 내의 캐릭터에게 대미지를 줌

PP_EffectBase 스킬들의 이펙트 클래스

PP_ProjectileAttack 직선으로 이동하며 단일 캐릭터에게 대미지를 줌

PP_ProjectileBoom 직선으로 이동하며 충돌 또는 일정 시간 후 주변의 캐릭터에게 대미지를 줌

<br /> 
<br /> 
<br /> 

## 4. UI
PP_MainWidgetFrame 클래스에서 뷰포트에 보여주는 다른 위젯클래스들을 관리하며, 플레이어 컨트롤러의 입력에 따라서 캐릭터 변경,장비착용, 상점, 몬스터공격등 동작에 따라서 PP_MainWidgetFrame을 통해서 변경사항을 적용 

# 4.2 코드

# 4.2.1 미니맵

미니맵을 촬영할 카메라를 포함한 PP_MiniMapCamOwner 클래스에서 주기적으로 태그를 통한 플레이어와 몬스터를 검색해 미니맵에 표시 합니다 

PP_MiniMapCamOwner::Tick

		if (delay_time <= 0)
		{
			//초기화
			if (IconWidgets.Num() != 0)
			{
				for (auto& Icons : IconWidgets)
				{
					Icons->RemoveFromParent();
				}
				IconWidgets.Reset();
			}

			//플레이어표시
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "PC", Actors);
			FVector ActorLoc = GetActorLocation();
			for (auto& player : Actors)
			{
				setIcon(ActorLoc, player->GetActorLocation());
			}
			
			//몬스터표시
			TArray<AActor*> Actors2;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Monster", Actors2);
			for (auto& mob : Actors2)
			{
				setIcon(ActorLoc, mob->GetActorLocation(), false);
			}


APP_MiniMapCamOwner::setIcon

	//범위내에 있는지 판단
	float x = pos.X - mid.X;
	if (2000 <= x || x <= -2000)
		return;
	float y = pos.Y - mid.Y;
	if (2000 <= y || y <= -2000)
		return;

	//좌표환산
	x = 200 + (x / 10);
	x = 400 - x;
	y = 200 + (y / 10);


	//아이콘 표시
	if (isPlayer)
	{
		IconWidgets.Add(Cast<UPP_MiniMapIcon>(CreateWidget(MiniMap, PlayerIcon)));
		IconWidgets[IconWidgets.Num()-1]->SetRenderTransform(FWidgetTransform(FVector2D(y, x), FVector2D(0.4f, 0.4f), FVector2D(0, 0), 0));
		MiniMap->GetPanel()->AddChild(IconWidgets[IconWidgets.Num() - 1]);
	}
	else
	{
		IconWidgets.Add(Cast<UPP_MiniMapIcon>(CreateWidget(MiniMap, MonsterIcon)));
		IconWidgets[IconWidgets.Num() - 1]->SetRenderTransform(FWidgetTransform(FVector2D(y, x), FVector2D(0.4f, 0.4f), FVector2D(0, 0), 0));
		MiniMap->GetPanel()->AddChild(IconWidgets[IconWidgets.Num() - 1]);
	}

# 4.2.2 퀘스트

![quest](https://user-images.githubusercontent.com/76766673/123458655-416ea580-d620-11eb-8104-f1748cd237b4.gif)

퀘스트 번호에 따라 퀘스트를 설정하고 특정 시간마다 거리체크또는 몬스터 사망 시 목표 몬스터와 비교해서 퀘스트 상태를 변경 합니다

	switch (quest_num)
	{
	case 1:
	{
		temp[0] = FString(TEXT("Goblin"));
		temp[1] = FString(TEXT("The castle was attacked by the demon army. Kill 15 goblins first"));
		SetQuest(true, 15, temp[0], temp[1], FName("Goblin"), MonsterType::Goblin); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	case 2:
	{
		temp[0] = FString(TEXT("Gate"));
		temp[1] = FString(TEXT("Killed all the monsters in the castle. let's go to the gate"));
		SetQuest(false, 0, temp[0], temp[1], FName("Door"), MonsterType::none, FVector(-24083.224609, 15460.894531, 8391.061523)); if (Quest_Board) Quest_Board->SetVisibility(ESlateVisibility::Visible);  break;
	}
	...
	
	UPP_Quset::CheckGoal(FVector pos)
	{
	Distance = 9999;
	if (!isBattle && !isClear)
	{
		Distance = FVector::Dist(pos, GoalPostion);
		if (Distance <= 300)
		{
			isClear = true;
			if (Quest_Board)
				Quest_Board->SetVisibility(ESlateVisibility::Collapsed);
			set_Quest_step_by_step();
		}
		else
		{
			if (Quest_Board)
			{
				Quest_Board->SetTargetText(FString::Printf(TEXT("Dist : %.3f"), Distance));
			}
		}
	}
	return Distance;
	
	...
	
	//퀘스트 해당 몬스터인지 체크
	if (type == TargetName)	
			{
				MonsterCnt++;
					if (MonsterCnt >= GoalNum)
					{
						isClear = true;
							if (Quest_Board)
								Quest_Board->SetVisibility(ESlateVisibility::Collapsed);
						set_Quest_step_by_step();
					}
					else
					{
						if (Quest_Board)
						{
							Quest_Board->SetTargetText(FString::Printf(TEXT("%d / %d"), MonsterCnt, GoalNum));
						}
					}
			}


# 4.2.3 아이템 장착

![equip](https://user-images.githubusercontent.com/76766673/123457908-60b90300-d61f-11eb-8667-d1b87edb41a1.gif)

인벤토리의 아이템을 클릭하면 해당 위젯의 소유자에게 해제한 후 소유자를 변경하는 방식으로 아이템을 착용 합니다.

	UPP_EquipItemWidgeet::EQItemClick()
	{
	//기존 아이템 소유자 초기화
	APP_Player* temp_owner = MyData->GetOwnerP();
	if (temp_owner)
	{
		temp_owner->SetItem(EquipItemType::none);
	}

	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
	{
		APP_Player* temp_player = temp_controller->GetControlCharacter();
		if (temp_player)
		{
			//아이템 소유자 전환
			temp_player->SetItem(MyData->GetMyItem());
			MyData->SetOwnerP(temp_player);
			MyData->SetOwner(temp_player->GetPlayerInfo()->Job);
		}
		//장비창 갱신
		temp_controller->ChangeEquip();
	}
	}


# 4.3 관련 클래스

PP_BuffIconWidget 독,마비,버프 상태를 나타내주는 클래스

PP_EquipItemData 장비 아이템의 정보를 담는 클래스

PP_EquipItemWidgeet 장비 아이템 1칸의 기능을 하는 클래스

PP_EquipWidgeet 장비 아이템을 리스트뷰로 보여주는 클래스

PP_InventoryWidget 장비창을 보여주는 클래스

PP_MainWidgetFrame 모든 위젯들은 담는 클래스

PP_MiniMapIcon 미니맵에 표시되는 녹색(플레이어)/적색(몬스터) 아이콘 클래스

PP_MiniMapCamOwner 미니맵 텍스쳐를 촬영할 카메라를 담은 클래스

PP_MiniMapWidget 미니맵 클래스

PP_PlayerPortraitWidget 캐릭터의 정보(HP,MP,EXP)를 보여주는 클래스

PP_QuestWidget 퀘스트의 정보를 보여주는 클래스

PP_Quset 퀘스트의 정보를 담는 클래스

PP_ShopItemData 상점의 아이템의 데이터를 담는 클래스

PP_ShopItemWidgeet 상정창의 1칸의 기능을 하는 클래스

PP_ShopWidget 상점을 리스트뷰로 보여주는 클래스

<br /> 
<br /> 
<br /> 

## 5. 기타

# 5.1 아이템박스

![obtain](https://user-images.githubusercontent.com/76766673/123457985-77f7f080-d61f-11eb-9d36-89cee7512fbf.gif)

각 몬스터들이 아이템 박스 클래스를 갖고 사망시 생성한 후 아래의 오버랩함수를 통해서 아이템 인덱스에 맞는 아이템을 인벤토리에 추가 하고 제거 합니다.

	APP_DropItem::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{
	APP_PlayerController* temp = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp)
	{
		//아이템 추가
		temp->ObtainItem(ItemIndex);
		if (ItemEffect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(ItemEffect, GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
	}
	Destroy();
	}

# 5.2 토클 스위치와 벽

![toggle](https://user-images.githubusercontent.com/76766673/123458035-85ad7600-d61f-11eb-856d-66d8ad2017a2.gif)

몬스터를 상속 받은 스위치 클래스에서 태그를 통해 담당 벽을 검색하고 벽의 이동시작 함수를 호출하면 벽의 Tick에서 벽의 위치를 변경 

	APP_PuzzleSwitch::TakeDamage
	if (FVector::Distance(DamageCauser->GetActorLocation(), GetActorLocation()) > 200)
		return 0;

	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TagName) ,Actors);
	for (auto& Wall : Actors)
	{
		APP_PuzzleSwitchWall* temp = Cast< APP_PuzzleSwitchWall>(Wall);
		if (temp)
			temp->ChangePos();
	}


	APP_PuzzleSwitchWall::Tick
	if (isMoving)
	{
		float deltapos = 500 * DeltaTime;
		if (IsFirst)
		{
			float temz = GetActorLocation().Z + deltapos;
			if (temz < OriginPos.Z + 500)
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, temz));
			}
			else
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, OriginPos.Z + 500));
				isMoving = false;
			}
		}
		else
		{
			float temz = GetActorLocation().Z - deltapos;
			if (temz > OriginPos.Z - 500)
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, temz));
			}
			else
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, OriginPos.Z - 500));
				isMoving = false;
			}
		}
	}

# 6. 일별 진행과정 

Day1

게임모드(PP_MainGameModeBase)생성
캐릭터클래스(PP_Player)생성 및 구조체(GameInstance에)정, 카메라 적용 (암달아서 회전,길이 조절)
컨트롤러클래스(PP_PlayerController) 생성 및 마우스클릭으로 캐릭터 이동 적용 (픽킹으로 좌표 획득 후 그 위치로 네비게이션AI로 이동)
검사(PP_ElfSwordMaster)생성, 메시 적용


Day2

캐릭터무기클래스(PP_Weapon) 생성, 메시와 이펙트 적용, 캐릭터1의 무기 적용
검사애님인스턴스생성(PP_ElfSwordMasterAnim), 각 상태에 따른 애니메이션 셋팅 (상태에 따른 블랜드, 여러 모션이 겹치는경우 스테이트 만들고 캐시)
캐릭터클래스와 검사클래스 가상함수(공격,스킬 등) 기본틀 작성, 힐러(PP_HighPristess)클래스 생성 후 기본틀 복붙

몬스터부모클래스(PP_Monster) 생성 및 구조체(GameInstance에)정의
고블린(PP_Goblin) 생성, 고블린애니메이션(PP_GoblinAnim)생성, 기본탐색 후 공격 패턴적용(유한상태기계)

몬스터와캐릭터 클래스에 TakeDamage함수 재정의
고블린의 기본공격을 통한 플레이어 TakeDamage호출


Day3

몬스터스폰포인트(MonsterSpawnPotin) 클래스 생성

/*
검사 캐릭터 AIController 및 비헤이비어 트리, 블랙보드 생성 (셀렉터,데코레이터,서비스,태스크를 이용한 이동, 기본공격, 방어 ,스킬 등)
//셀럭터의 서비스로 상황판단, 데코레이터로 조건 판단, 태스크로 수행
플레이어컨트롤러와 겹치기 때문에 취소
*/

이동에서 발생하는 버그 수정 (플레이어 컨트롤러가 다른 캐릭터에 있을 시 생기는 버그 = Relative World간 문제)
플레이어 가드전환 함수추가
플레이어 자동전투 함수추가

고블린 기본공격에 트레일 작업 추가
플레이어 기본공격에 트레일, 타격이펙트(랜덤회전) 추가

조작 UI 버튼추가작업
조작캐릭터의 자동전투 on/off키, 가드키
오토배틀 <-> 가드 사이에서 일어나는 버그 발견 및 수정
나머지 버튼 델리게이트 연결


Day4

충돌시 폭발현상 제거 (메시를 노콜리전으로)
오토배틀 <-> 가드 <-> 플레이어조작이동 3개 상태에서 일어나는 오류 수정

픽킹을 통한 기본공격 대상 변경하기 추가
검사 기본공격 범위에 여러마리 몬스터가 있을 시 같이 동시 타격하도록 수정

검사 스킬1(범위내 어그로끌기)완성, 범위는 임시값

나머지 스킬2~5 트레일, 이펙트등 노티파이 작업


Day5

스킬들 구현을 위한 몬스터 및 캐릭터 정보 초기화, TakeDamage에서 HP감소, 죽음 EndPlay재정의 등
몬스터스폰포인트에서 리스폰 및 최대 리스폰 횟수 추가
플레이어 자동전투 타겟전환, 고블린사망모션 오류 수정 (pp고블린 틱,몬스터테이크대미지,엘프공격)
검사,고블린 공격에 사운드추가
검사스킬 2 3 4 5 구현


Day6

상시 상태창(Hp,Mp,Exp)구현 및 플레이어+검사클래스에 HP변경 적용 완료(HP영향 주는 부분에서 함수호출)
몬스터이름과 HP를 보여주는 위젯 구현 (플레이어 컨트롤러의 타겟을 추적)
포션1,2 구현 (3,4는 추후 상태이상이 나오면)


Day7

다중캐릭터 조작 및 UI연결 구현
잡던 몬스터를 다른 캐릭터가 잡으면 버그 -> 해결
캐릭터 자동전투 디텍트 버그 -> 해결
!!!!!!!!!!!다이나믹 + 캐릭터들이 네비메시에 영향주게 체크 해도 이상함!!!!!!!


Day8

투사체 공격 구현
스킬아이콘 전환 구현
힐러 캐릭터 정의 및 아이템사용까지


Day9

힐러 전체 스킬 2,5구현
힐러 타겟팅 스킬 1,3,4 구현
!!!! 나중에 3상태이상 4부활은 따로 코드 추가해야함 !!!!


Day10

장판기 클래스 구현
원딜 캐릭터 구현


Day11

퀘스트 시스템완료
미니맵 구현


Day12

힐러오크 구현완료
장비창 및 인벤토리-장비 구현완료


Day13

워리어오크 구현완료 및 플레이어 마비(경직)상태이상 추가


Day14

UI 디자인 개선
포스트프로세스 머티리얼 추가
드롭아이템 구현


Day15

스테이지1 구성 완료


Day16

캐릭터 사망 및 몬스터타겟검색(사망시 갱신) 추가
경험치 및 레벨업 추가
캐릭터 사망 애니메이션 수정


Day17

비선공 몬스터 스콜피 추가
스테이지 보스2의 소환거미 추가
맵전환 완료 (아이템 그냥 일괄해제)


Day18

오류 수정


Day19

맵2 보스씬 완성
장비까지 레벨전환 완료
맵2 보스 스킬완성


Day20

맵2 보스적용완료
상점구현완료


Day21

스테이지3 잡몹1 추가 (공격에 랜덤 상태이상)
스테이지3 맵오류 카메라 예외적으로 카메라 높이 수정으로 땜빠
독 상태이상 구현


Day22

서큐버스 구현완료


Day23

사탄 구현완료
스테이지4 맵 구성


Day24

최종보스 구현완료
스테이지1~4연결 완료


Day25

버프,디버프 아이콘 추가
스킬쿨타임 추가


Day26

퍼즐요소 추가


