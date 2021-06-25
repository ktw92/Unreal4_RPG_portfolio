// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_Player.h"
#include "PP_PlayerAIController.h"
#include "PP_MainGameModeBase.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KTWFunc.h"
#include "PP_Monster.h"
#include "PP_PlayerPortraitWidget.h"
#include "PP_ElfSwordMaster.h"
#include "PP_HighPriestess.h"
#include "PP_Wizard.h"
#include "PP_PlayerController.h"
#include "PP_MainWidgetFrame.h"

// Sets default values
APP_Player::APP_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_Arm);
	m_Arm->SetupAttachment(GetCapsuleComponent());
	m_Arm->TargetArmLength = 750.f;
	m_Arm->TargetOffset.Z = 140.f;

	GetCapsuleComponent()->SetCollisionProfileName("PlayerBody");

	Tags.Add("PC");

	//포션이펙트
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset1(TEXT("Blueprint'/Game/0_PP/0_Player/PotionParticle/PP_BPHpPotionEffect.PP_BPHpPotionEffect_C'"));
	if (EffectAsset1.Succeeded())
		HpEffect = EffectAsset1.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Player/PotionParticle/PP_BPMpPotionEffect.PP_BPMpPotionEffect_C'"));
	if (EffectAsset2.Succeeded())
		MpEffect = EffectAsset2.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset3(TEXT("Blueprint'/Game/0_PP/0_Player/PotionParticle/PP_BPRecoverPotionEffect.PP_BPRecoverPotionEffect_C'"));
	if (EffectAsset3.Succeeded())
		DebufRecoverEffect = EffectAsset3.Class;
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset4(TEXT("Blueprint'/Game/0_PP/0_Player/PotionParticle/PP_BPReraiserPotionEffect.PP_BPReraiserPotionEffect_C'"));
	if (EffectAsset4.Succeeded())
		ReraiseEffect = EffectAsset4.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset5(TEXT("Blueprint'/Game/0_PP/0_Player/PP_LevelUpEffect.PP_LevelUpEffect_C'"));
	if (EffectAsset5.Succeeded())
		LevelUpEffect = EffectAsset5.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset6(TEXT("Blueprint'/Game/0_PP/0_Player/PotionParticle/PP_PoisonEffectBP.PP_PoisonEffectBP_C'"));
	if (EffectAsset6.Succeeded())
		Poison = EffectAsset6.Class;

	

	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	isMoving = false;
	isGuard = false;
	isSKill = false;
	isAutoBattle = false;
	isAssemble = false;
	TargetSkillNum = 0;
	SKillTargetFree();
	PlayerInfo.MyItem = EquipItemType::none;

	GetMesh()->SetCustomDepthStencilValue(0);
	GetMesh()->SetRenderCustomDepth(true);


	PlayerInfo.Skill_cooltime[0] = PlayerInfo.Skill_cooltime[1] = PlayerInfo.Skill_cooltime[2] = PlayerInfo.Skill_cooltime[3] = PlayerInfo.Skill_cooltime[4] = 3.f;
	for (int i = 0; i < 5; i++)
	{
		PlayerInfo.Skill_cooltimeAcc[i] = 5;
	}
}

// Called when the game starts or when spawned
void APP_Player::BeginPlay()
{
	Super::BeginPlay();
	//폭발제거
	GetMesh()->SetCollisionProfileName("NoColButMouseOk");

	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	//bFindCameraComponentWhenViewTarget = false;
	CameraRot = FRotator::ZeroRotator;

}

// Called every frame
void APP_Player::Tick(float DeltaTime)
{
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

	//임시땜빵 카메라매니저, 모디파이등 처리해서 해야할지도
	m_Arm->SetWorldRotation(CameraRot);
	Super::Tick(DeltaTime);
	//임시땜빵
	if (!GetController())
		return;

	if (isGuard || isSKill || isUsingItem)
		return;

	if (PlayerInfo.isDead)
	{
		GetController()->StopMovement();
		return;
	}

	if (PlayerInfo.Debuff[0] == true)//마비상태면
	{
		PlayerInfo.Paralysis_Acc += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("pala time %f / %f"), MonsterStatus.Paralysis_Acc, ParalysisTime));
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
			/*
#if ENABLE_DRAW_DEBUG
			FColor	DrawColor = bSweep ? FColor::Red : FColor::Green;
			DrawDebugLine(GetWorld(), GetActorLocation(), endp,
				DrawColor, false, 3.f);
#endif
*/
			GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));
		}
		else
		{
			//목표로 회전
			FVector Dir = MoveGoal - GetActorLocation();
			Dir.Normalize();
			GetMesh()->SetWorldRotation(FRotator(0.f, Dir.Rotation().Yaw - 90.f, 0.f));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("AI Move")));
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
			/*
			APP_Monster* temp_mon = Cast< APP_Monster>(TargetedMonster);
			if (temp_mon->GetStatus()->Hp <= 0)//시체인지 확인
			{
				SimpleStop();
				TargetFree();
				return;
			}
			*/
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
						//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("no attack1 - AttackAttackAttack")));
					}
					else //공격중거리에 없으면 이동
					{
						AActor* tempm = TargetedMonster;
						SimpleMove(TargetedMonster->GetActorLocation());
						TargetedMonster = tempm;
						//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("no attack1 - movemovemove")));
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
					//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("so far move")));
				}
				//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("ok attack1")));
			}
			//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("set rot")));
			FVector Dir = TargetedMonster->GetActorLocation() - GetActorLocation();
			Dir.Normalize();
			GetMesh()->SetWorldRotation(FRotator(0.f, Dir.Rotation().Yaw - 90.f, 0.f));
		}
		else //타겟 없으면 탐지
		{
			//TArray<FHitResult> temp = BoxDetectMulti(this->GetActorLocation(), GetMesh()->GetForwardVector(), PlayerInfo.Attack_range * 20, GetWorld(), this, ECC_GameTraceChannel3, true);
			TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), PlayerInfo.Attack_range * 20, GetWorld(), this, ECC_GameTraceChannel3, true);
			//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("find")));
			float mindist = 123456789;
			for (auto& hitted : temp)
			{
				APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
				if (HittedMonster)
				{
					//시체는 안건드림
					if (HittedMonster->GetStatus()->Hp <= 0)
					{
						//TargetFree();
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
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("Target Autobattle")));
	}
	else if (isAssemble) //다 끝내고 집합중
	{
	isAssemble;
	}
	else
	{
		SimpleStop();
	/*
		GetController()->StopMovement();
		isMoving = false;
		MoveGoal = FVector::ZeroVector;*/
	}
}

// Called to bind functionality to input
void APP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*
	PlayerInputComponent->BindAxis(TEXT("CameraTurn"), this, &APP_Player::CameraTurn);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &APP_Player::CameraZoom);
	PlayerInputComponent->BindAxis(TEXT("CameraLookUp"), this, &APP_Player::CameraLookUp);
	*/
}

void APP_Player::CameraTurn(float fScale)
{
	fScale *= 180;
	//AddControllerYawInput(fScale);
	//m_Arm->AddRelativeRotation(FRotator(0.f, fScale * GetWorld()->GetDeltaSeconds(), 0.f));
	CameraRot.Add(0.f, fScale * GetWorld()->GetDeltaSeconds(), 0.f);
	m_Arm->SetWorldRotation(CameraRot);
}
void APP_Player::CameraZoom(float fScale)
{
	if ( (m_Arm->TargetArmLength <= 3000 && fScale > 0) || (500 <= m_Arm->TargetArmLength && fScale < 0) )
	{
		fScale *= 300;
		m_Arm->TargetArmLength += fScale;
	}
}
void APP_Player::CameraLookUp(float fScale)
{
	fScale *= 180;
	//m_Arm->AddRelativeRotation(FRotator(fScale * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
	CameraRot.Add(fScale * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);
	m_Arm->SetWorldRotation(CameraRot);
}

void APP_Player::SimpleMove(FVector loc)
{
	//UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("simple move")));
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("%f %f %f"), loc.X, loc.Y, loc.Z));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), loc);
	ChangeAnimState(AnimType::Move);
	isMoving = true;
	isGuard = false;
	isSKill = false;
	MoveGoal = loc;
	TargetFree();
}

void APP_Player::SimpleStop()
{
	if(PlayerInfo.AnimState == AnimType::Move || PlayerInfo.AnimState == AnimType::Attack1)
		ChangeAnimState(AnimType::Idle);
	GetController()->StopMovement();
	isMoving = false;
	MoveGoal = FVector::ZeroVector;
}


float APP_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float fDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (PlayerInfo.Hp <= 0)
		return fDamage;

	//대미지 처리
	float temp_dmg = DamageAmount - PlayerInfo.Defense[0];
	if (temp_dmg < 0)
		temp_dmg = 0;
	if (PlayerInfo.AnimState == AnimType::Guard)
		temp_dmg = temp_dmg / 2;
	PlayerInfo.Hp -= temp_dmg;

	if (PlayerInfo.Hp <= 0)
	{
		PlayerInfo.Hp = 0;
		PlayerInfo.isDead = true;
		PlayerInfo.Debuff[0] = PlayerInfo.Debuff[1] = false;
		PlayerInfo.Paralysis_Acc = 0;
		SetIdle();
		ChangeAnimState(AnimType::Death);
	}

	if (MyPortraitWidget)
		MyPortraitWidget->SetHpPercent(PlayerInfo.Hp * 1.0 / PlayerInfo.Max_hp);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player Take Damage Hp : %d"), PlayerInfo.Hp));

	return fDamage;
}

void APP_Player::SetAutoBattleOnOff(bool isParty, bool isOn)
{
	if(!isParty) //싱슬
	{
		if (!isOn)
		{
			isAutoBattle = false;
			return;
		}
		isAutoBattle = !isAutoBattle;
		if (isAutoBattle)
		{
			;//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Autobattle single on")));
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Autobattle single off")));
			SKillTargetFree(); ItemTargetFree(); isSKill = false;
			//SetIdle();
		}
	}
	else //파티강제전환
	{
		if (isOn)
		{
			isAutoBattle = true;
		}
		else
		{
			isAutoBattle = false;
			SKillTargetFree(); ItemTargetFree(); isSKill = false;
			//SetIdle();
		}
	}
}

void APP_Player::Guard(bool isParty, bool isOn)
{
	if (!isParty) //싱글
	{
		if (isGuard)
		{
			ChangeAnimState(AnimType::Idle);
			isGuard = false;
		}
		else
		{
			GetController()->StopMovement();
			ChangeAnimState(AnimType::Guard);
			isMoving = false;
			isGuard = true;
			isSKill = false;
			TargetedMonster = nullptr;
		}
	}
	else //파티강제전환
	{
		if (isOn)
		{
			GetController()->StopMovement();
			ChangeAnimState(AnimType::Guard);
			isMoving = false;
			isGuard = true;
			isSKill = false;
			TargetedMonster = nullptr;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Guard Party off")));
			ChangeAnimState(AnimType::Idle);
			isGuard = false;
		}
	}
}

void APP_Player::SetTargetMonster(AActor* target)
{
	TargetedMonster = target;
}

void APP_Player::UsingItemFire()
{
	switch (ItemType)
	{
	case ConsumeItemType::Hp_potion:
	{
		//아이템수 체크 필요
		if (HpEffect && ItemTarget)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(HpEffect, ItemTarget->GetActorLocation(),
					FRotator::ZeroRotator, param);
			//조절필요
			ItemTarget->HpUp(500);
			ItemTarget = nullptr;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Hp potion use complete")));
		}		
	}
		break;
	case ConsumeItemType::Mp_potion:
	{
		//아이템수 체크 필요
		if (MpEffect && ItemTarget)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(MpEffect, ItemTarget->GetActorLocation(),
				FRotator::ZeroRotator, param);
			//조절필요
			ItemTarget->MpUp(500);
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Mp potion use complete")));
		}
	}
		break;
	case ConsumeItemType::Debuff_recover:
	{
		//아이템수 체크 필요
		if (DebufRecoverEffect && ItemTarget)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(DebufRecoverEffect, ItemTarget->GetActorLocation(),
				FRotator::ZeroRotator, param);

			if (ItemTarget->GetPlayerInfo()->Debuff[0] == true)
			{
				APP_ElfSwordMaster* temp_knight = Cast<APP_ElfSwordMaster>(ItemTarget);
				APP_HighPriestess* temp_pristess = Cast<APP_HighPriestess>(ItemTarget);
				APP_Wizard* temp_wizard = Cast<APP_Wizard>(ItemTarget);
				if (temp_knight)
				{
					temp_knight->SetIdle();
				}
				else if (temp_pristess)
				{
					temp_pristess->SetIdle();
				}
				else if (temp_wizard)
				{
					temp_wizard->SetIdle();
				}
			}

			for (int i = 0; i < 2; i++)
			{
				if (ItemTarget->GetPortrait())
					ItemTarget->GetPortrait()->ReSetDenuff(i);
				ItemTarget->GetPlayerInfo()->Debuff[i] = false;
			}
			ItemTarget->GetPlayerInfo()->Paralysis_Acc = 0;
			ItemTarget->GetPlayerInfo()->Poison_Acc = 0;

		}
	}
		break;
	case ConsumeItemType::Resurrectioner:
		if (ReraiseEffect && ItemTarget)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(ReraiseEffect, ItemTarget->GetActorLocation(),
				FRotator::ZeroRotator, param);

			ItemTarget->GetPlayerInfo()->Hp = ItemTarget->GetPlayerInfo()->Max_hp / 2 - 100;
			ItemTarget->HpUp(100);

			APP_ElfSwordMaster* temp_knight = Cast<APP_ElfSwordMaster>(ItemTarget);
			APP_HighPriestess* temp_pristess = Cast<APP_HighPriestess>(ItemTarget);
			APP_Wizard* temp_wizard = Cast<APP_Wizard>(ItemTarget);
			if (temp_knight)
			{
				temp_knight->Reraise();
			}
			else if (temp_pristess)
			{
				temp_pristess->Reraise();
			}
			else if (temp_wizard)
			{
				temp_wizard->Reraise();
			}
		}
		break;
	default:
		break;
	}
}

bool APP_Player::UseMp(int amount)
{
	if (PlayerInfo.Mp < amount)
	{
		SetIdle();
		return false;
	}
	PlayerInfo.Mp -= amount;
	if (MyPortraitWidget)
		MyPortraitWidget->SetMpPercent(PlayerInfo.Mp * 1.0 / PlayerInfo.Max_mp);
	return true;
}

bool APP_Player::MpUp(int amount)
{
	PlayerInfo.Mp += amount;
	if (PlayerInfo.Mp > PlayerInfo.Max_mp)
	{
		PlayerInfo.Mp = PlayerInfo.Max_mp;
	}
	if (MyPortraitWidget)
		MyPortraitWidget->SetMpPercent(PlayerInfo.Mp * 1.0 / PlayerInfo.Max_mp);
	return true;
}

bool APP_Player::HpUp(int amount)
{
	PlayerInfo.Hp += amount;
	if (PlayerInfo.Hp > PlayerInfo.Max_hp)
	{
		PlayerInfo.Hp = PlayerInfo.Max_hp;
	}
	if (MyPortraitWidget)
		MyPortraitWidget->SetHpPercent(PlayerInfo.Hp * 1.0 / PlayerInfo.Max_hp);
	return true;
}

bool APP_Player::HpDown(int amount)
{
	PlayerInfo.Hp -= amount;
	if (PlayerInfo.Hp < 0)
	{
		PlayerInfo.Hp = 0;
	}
	if (MyPortraitWidget)
		MyPortraitWidget->SetHpPercent(PlayerInfo.Hp * 1.0 / PlayerInfo.Max_hp);

	if (PlayerInfo.Hp <= 0)
	{
		PlayerInfo.Hp = 0;
		PlayerInfo.isDead = true;
		PlayerInfo.Debuff[0] = PlayerInfo.Debuff[1] = false;
		PlayerInfo.Paralysis_Acc = 0;
		SetIdle();
		ChangeAnimState(AnimType::Death);
	}
	return true;
}

FVector APP_Player::GetMeshForwardVector()
{
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	FVector rst = GetMesh()->GetForwardVector();
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));
	return rst;
}

void APP_Player::SetItem(EquipItemType type)
{
	//기존아이템 해제
	switch (PlayerInfo.MyItem)
	{
	case EquipItemType::none: break;
	case EquipItemType::Defense:
	{
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] - 2000;
		break;
	}
	case EquipItemType::Power:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] - 2000;
		break;
	}
	case EquipItemType::SoftPower:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] - 1000;
		break;
	}
	case EquipItemType::SoftDefense:
	{
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] - 1000;
		break;
	}
	case EquipItemType::Mix:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] - 1500;
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] - 1500;
		break;
	}
	default: break;
	}

	PlayerInfo.MyItem = type;

	//아이템 적용
	switch (PlayerInfo.MyItem)
	{
	case EquipItemType::none: break;
	case EquipItemType::Defense:
	{
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] + 2000;
		break;
	}
	case EquipItemType::Power:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] + 2000;
		break;
	}
	case EquipItemType::SoftPower:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] + 1000;
		break;
	}
	case EquipItemType::SoftDefense:
	{
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] + 1000;
		break;
	}
	case EquipItemType::Mix:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] + 1500;
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] + 1500;
		break;
	}
	default: break;
	}
}

void APP_Player::RemoveEquipItem()
{
	switch (PlayerInfo.MyItem)
	{
	case EquipItemType::none: break;
	case EquipItemType::Defense:
	{
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] - 2000;
		break;
	}
	case EquipItemType::Power:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] - 2000;
		break;
	}
	case EquipItemType::SoftPower:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] - 1000;
		break;
	}
	case EquipItemType::SoftDefense:
	{
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] - 1000;
		break;
	}
	case EquipItemType::Mix:
	{
		PlayerInfo.Attack[0] = PlayerInfo.Attack[0] - 1500;
		PlayerInfo.Defense[0] = PlayerInfo.Defense[0] - 1500;
		break;
	}
	default: break;
	}
	PlayerInfo.MyItem = EquipItemType::none;
}


void APP_Player::ObtainExp(int amount)
{
	PlayerInfo.Exp += amount;
	LevelUP();
	HpUp(0);
	MpUp(0);
	if (MyPortraitWidget)
		MyPortraitWidget->SetExpPercent(PlayerInfo.Exp / 5000.f);
}

void APP_Player::LevelUP()
{
	if (PlayerInfo.Exp >= 5000)
	{
		if (LevelUpEffect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GetWorld()->SpawnActor<APP_EffectBase>(LevelUpEffect, GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
	}
}

void APP_Player::SetPoison()
{
	if (PlayerInfo.Hp > 0)
	{
		if (MyPortraitWidget)
		{
			MyPortraitWidget->ReSetDenuff(1);
			MyPortraitWidget->SetDeBuff(1, PoisonTime);
		}
		PlayerInfo.Debuff[1] = true;
		PlayerInfo.Poison_Acc = 0.f;
	}
}