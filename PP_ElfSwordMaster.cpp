// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ElfSwordMaster.h"
#include "PP_ElfSwordMasterAIController.h"
#include "KTWFunc.h"
#include "PP_MainGameModeBase.h"
#include "PP_PlayerPortraitWidget.h"
#include "PP_MainWidgetFrame.h"
#include "Kismet/GameplayStatics.h"
#include "PP_PlayerController.h"



APP_ElfSwordMaster::APP_ElfSwordMaster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	MeshAsset(TEXT("SkeletalMesh'/Game/Elf_Swordmaster/Meshes/ElfSwordmaster_FullArmor.ElfSwordmaster_FullArmor'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	//애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Player/ElfSwordMaster/BP_PPElfSwordMasterAnim.BP_PPElfSwordMasterAnim_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	//장착무기
	static ConstructorHelpers::FClassFinder<APP_Weapon>	WE(TEXT("Blueprint'/Game/0_PP/0_Player/ElfSwordMaster/BPElfSwordEffect.BPElfSwordEffect_C'"));
	if (WE.Succeeded())
		WeaponWithEffect = WE.Class;
	//기본공격 이펙트
	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset(TEXT("Blueprint'/Game/0_PP/0_Player/ElfSwordMaster/PP_ElfSwordMasterNormalAttaackEffect.PP_ElfSwordMasterNormalAttaackEffect_C'"));
	if (EffectAsset.Succeeded())
		NormalHitEffect = EffectAsset.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset2(TEXT("Blueprint'/Game/0_PP/0_Player/ElfSwordMaster/PP_ElfSwordMasterSkill2.PP_ElfSwordMasterSkill2_C'"));
	if (EffectAsset2.Succeeded())
		Skill2HitEffect = EffectAsset2.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset4(TEXT("Blueprint'/Game/0_PP/0_Player/ElfSwordMaster/PP_ElfSwordMasterSkill4.PP_ElfSwordMasterSkill4_C'"));
	if (EffectAsset4.Succeeded())
		Skill4HitEffect = EffectAsset4.Class;

	static ConstructorHelpers::FClassFinder<APP_EffectBase>	EffectAsset5(TEXT("Blueprint'/Game/0_PP/0_Player/ElfSwordMaster/PP_ElfSwordMasterSkill5.PP_ElfSwordMasterSkill5_C'"));
	if (EffectAsset5.Succeeded())
		Skill5HitEffect = EffectAsset5.Class;

	GetMesh()->SetCollisionProfileName("NoCollision");
	//AI
	/*
	AIControllerClass = APP_ElfSwordMasterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	*/
	PlayerInfo.Job = FString::Printf(TEXT("Knight"));

	//비긴플레이에서 옮겨옴
	/*
	//컨트롤로에 추가, 포트레잇UI 등록
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
		PlayerInfo.PortraitNum = temp_controller->PlayerInsert(this);
	//전사는 맨위
	PlayerInfo.PortraitNum = 1;
	*/
}

void APP_ElfSwordMaster::BeginPlay()
{
	Super::BeginPlay();

	//겹침 폭발제거
	//GetMesh()->SetCollisionProfileName("NoCollision");

	//캡슐에 맞게 메시 위치,회전조절
	GetMesh()->AddRelativeLocation(FVector(0, 0, -90.f)); //메시 위치에 따른 이동오류 -> 블루프린트로 세부조절 대체왜? -> 무기충돌체
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//무기장착
	if (WeaponWithEffect)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ElfWeapon = GetWorld()->SpawnActor<APP_Weapon>(WeaponWithEffect, FVector::ZeroVector,
			FRotator::ZeroRotator, param);

		ElfWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("hand_rSocket"));

		ElfWeapon->EffectOnOff(false);
	}
	
	//상태초기화
	MyAnim = Cast<UPP_ElfSwordMasterAnim>(GetMesh()->GetAnimInstance());

	if(MyAnim)
		ChangeAnimState(AnimType::Idle);

	
	//컨트롤로에 추가, 포트레잇UI 등록
	APP_PlayerController* temp_controller = Cast<APP_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (temp_controller)
		PlayerInfo.PortraitNum = temp_controller->PlayerInsert(this);
	//전사는 맨위
	PlayerInfo.PortraitNum = 1;


	//데이터 읽어오기
	UPP_GameInstance* tempInstance = Cast<UPP_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (tempInstance)
	{
		if (tempInstance->IsSave())
		{
			FPP_PlayerInfo* tempInfo = tempInstance->GetPlayerInfo(PlayerInfo.PortraitNum-1);
			if (tempInfo)
			{
				PlayerInfo.Attack_range = tempInfo->Attack_range;
				PlayerInfo.Attack[0] = tempInfo->Attack[0];
				PlayerInfo.Defense[0] = tempInfo->Defense[0];
				PlayerInfo.Hp = tempInfo->Hp;					
				PlayerInfo.Max_hp = tempInfo->Max_hp;
				PlayerInfo.Mp = tempInfo->Mp;
				PlayerInfo.Max_mp = tempInfo->Max_mp;
				PlayerInfo.Exp = tempInfo->Exp;
			}
		}
		else
		{
			//캐릭터정보 초기화, 나중에 파일읽기로 변경
			PlayerInfo.Attack_range = 250.f;
			PlayerInfo.Attack[0] = 300;
			PlayerInfo.Defense[0] = 50;
			PlayerInfo.Hp = PlayerInfo.Max_hp = 2000;
			PlayerInfo.Mp = PlayerInfo.Max_mp = 1000;
			PlayerInfo.Exp = 0;
		}
	}



	isSkill2 = false;
	SKill2_Damage = 0;

	isSkill3 = false;
	Skill3Time = 8.f;
	Skill3Acc = 0.f;
	IntSkill3Acc = 0;

	//상태위젯과 연결
	APP_MainGameModeBase* temp_gamemode = Cast<APP_MainGameModeBase>(GetWorld()->GetAuthGameMode());
	if (temp_gamemode)
	{
		UPP_MainWidgetFrame* temp_mainwidget = Cast<UPP_MainWidgetFrame>(temp_gamemode->GetMainWidget());
		if (temp_mainwidget)
		{
			UPP_PlayerPortraitWidget* temp_portraitWidget = temp_mainwidget->GetPortraitWidget(PlayerInfo.PortraitNum);
			if (temp_portraitWidget)
			{
				MyPortraitWidget = temp_portraitWidget;
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("PortraitWidget Load complete")));
				MyPortraitWidget->SetExpPercent(PlayerInfo.Exp / 5000.f);
				MyPortraitWidget->SetHpPercent(PlayerInfo.Hp / (float)(PlayerInfo.Max_hp));
				MyPortraitWidget->SetMpPercent(PlayerInfo.Mp / (float)(PlayerInfo.Max_mp));
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("port none")));
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Mainmode none")));
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("gamemode none")));
	}
}

// Called every frame
void APP_ElfSwordMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isSkill3)
	{
		//n초당 최대체력의 퍼센트 감소
		Skill3Acc += DeltaTime;
		int tempi = (int)(Skill3Acc);
		if (tempi != IntSkill3Acc)
		{
			IntSkill3Acc = tempi;
			PlayerInfo.Hp -= PlayerInfo.Max_hp * 0.05f;
			if (MyPortraitWidget)
				MyPortraitWidget->SetHpPercent(PlayerInfo.Hp * 1.0 / PlayerInfo.Max_hp);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Skill3 %d hp : %d"), IntSkill3Acc, PlayerInfo.Hp));
		}
		//최대시간이면 해제
		if (Skill3Acc >= Skill3Time)
		{
			Skill3Acc = 0.f;
			isSkill3 = false;
			if (ElfWeapon)
				ElfWeapon->EffectOnOff(false);
		}
	}

	if (PlayerInfo.isDead)
	{
		if (PlayerInfo.AnimState != AnimType::Reraise)
			ChangeAnimState(AnimType::Death);
		GetController()->StopMovement();
		return;
	}



	if (PlayerInfo.Debuff[0] == true)//마비상태
	{
		ChangeAnimState(AnimType::Paralysis);
		return;
	}


	//스킬4, 타겟팅 스킬이 1개이기 때문에 스킬타겟의 존재 여부로 스킬 발동판단
	if (SkillTarget)
	{
		isSKill = true;
		if (PlayerInfo.AnimState != AnimType::Skill4)
		{
			float dist = FVector::Dist(GetActorLocation(), SkillTarget->GetActorLocation());
			if (dist <= PlayerInfo.Attack_range * 0.8) //공격거리에 있으면 공격
			{
				FVector dir = SkillTarget->GetActorLocation() - GetActorLocation();
				dir.Normalize();
				GetMesh()->SetWorldRotation(FRotator(0.f, dir.Rotation().Yaw - 90.f, 0.f));
				SimpleStop();
				ChangeSkill4();
			}
			else //공격중거리에 없으면 이동
			{
				AActor* tempm = SkillTarget;
				SimpleMove(SkillTarget->GetActorLocation());
				SkillTarget = tempm;
			}
		}
	}
}

void APP_ElfSwordMaster::SimpleMove(FVector roc)
{
	Super::SimpleMove(roc);
}
void APP_ElfSwordMaster::Guard(bool isParty, bool isOn)
{
	Super::Guard(isParty, isOn);
}
void APP_ElfSwordMaster::NormalAttack()
{
	Super::NormalAttack();

	//
	/*
	//단일공격
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	FHitResult temp = ForwardBoxOneDetect(this->GetActorLocation(), GetMesh()->GetForwardVector(), PlayerInfo.Attack_range, GetWorld(), this, ECC_GameTraceChannel3, true);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	APP_Monster* HittedMonster = Cast<APP_Monster>(temp.GetActor());
	if (HittedMonster)
	{
		if (NormalHitEffect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//기본공격 이펙트 랜더회전
			FRandomStream Rs;
			Rs.GenerateNewSeed();
			FRotator randRot;
			randRot.Yaw = Rs.FRandRange(0.f, 180);
			randRot.Pitch = Rs.FRandRange(0.f, 180);
			randRot.Roll = Rs.FRandRange(0.f, 180);

			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));

			GetWorld()->SpawnActor<APP_EffectBase>(NormalHitEffect, HittedMonster->GetActorLocation(),
				randRot, param);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
		}
		FDamageEvent	DmgEvent;
		HittedMonster->TakeDamage(1.f, DmgEvent, GetController(), this);
	}
	*/

	//다중공격
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	TArray<FHitResult> temp = ForwardBoxOneDetectMulti(this->GetActorLocation(), GetMesh()->GetForwardVector(), PlayerInfo.Attack_range, GetWorld(), this, ECC_GameTraceChannel3, true);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	for (auto& hitted : temp)
	{
		APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
		if (HittedMonster)
		{	
			//시체는 안건드림
			if (HittedMonster->GetStatus()->Hp <= 0)
			{
				//HittedMonster->ChangeAnimState(AnimType::Death);
				continue;
			}

			if (NormalHitEffect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				//기본공격 이펙트 랜더회전
				FRandomStream Rs;
				Rs.GenerateNewSeed();
				FRotator randRot;
				randRot.Yaw = Rs.FRandRange(0.f, 180);
				randRot.Pitch = Rs.FRandRange(0.f, 180);
				randRot.Roll = Rs.FRandRange(0.f, 180);

				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));

				GetWorld()->SpawnActor<APP_EffectBase>(NormalHitEffect, HittedMonster->GetActorLocation(),
					randRot, param);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
			}
			
			FDamageEvent	DmgEvent;

			float DamageAmount = PlayerInfo.Attack[0];
			if (isSkill3)
				DamageAmount *= 2;

			float temp_monstr_hp = HittedMonster->TakeDamage(DamageAmount, DmgEvent, GetController(), this);
			if (HittedMonster == TargetedMonster)
				if (temp_monstr_hp < 0)
					TargetFree();
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("elfswordmaster normal attack end")));
}

void APP_ElfSwordMaster::Skill1()
{
	if (!UseMp(100))
	{
		SetIdle();
		return;
	}

	//Super::Skill1();
	//스킬범위 설정 필요함
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	//TArray<FHitResult> temp = ForwardBoxOneDetectMulti(this->GetActorLocation(), GetMesh()->GetForwardVector(), 3000.f, GetWorld(), this, ECC_GameTraceChannel3, true);
	TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), PlayerInfo.Attack_range * 10, GetWorld(), this, ECC_GameTraceChannel3, true);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	for (auto& hitted : temp)
	{
		APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
		if (HittedMonster)
		{
			AActor* tempap = Cast<AActor>(this);
			if(tempap)
				HittedMonster->SetTarget(tempap);
		}
	}
}
void APP_ElfSwordMaster::Skill2()
{
	Super::Skill2();
}
void APP_ElfSwordMaster::Skill3()
{
	if (!UseMp(200))
	{
		SetIdle();
		return;
	}

	Super::Skill3();
	if (MyPortraitWidget)
	{
		MyPortraitWidget->SetBuff1(Skill3Time);
	}

	isSkill3 = true;
	Skill3Acc =  0.f;
	IntSkill3Acc = 0;
	if(ElfWeapon)
		ElfWeapon->EffectOnOff(true);
}

void APP_ElfSwordMaster::Skill4Start()
{
	if (!UseMp(400))
	{
		SetIdle();
		return;
	}
}

void APP_ElfSwordMaster::Skill4()
{

	Super::Skill4();
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill4 Fire")));
	
	//정면으로 단일개체만 판정
	FHitResult	result;
	FCollisionQueryParams	params(NAME_None, false, this);
	params.bReturnPhysicalMaterial = true;
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw + 90.f, 0.f));
	bool bSweep = GetWorld()->LineTraceSingleByChannel(result, GetActorLocation(), GetActorLocation() + GetMesh()->GetForwardVector() * PlayerInfo.Attack_range * 1.2f,
		ECC_GameTraceChannel3, params);
	GetMesh()->SetRelativeRotation(FRotator(0.f, GetMesh()->GetRelativeRotation().Yaw - 90.f, 0.f));

	APP_Monster* HittedMonster = Cast<APP_Monster>(result.GetActor());
	if (HittedMonster)
	{
		//시체
		if (HittedMonster->GetStatus()->Hp <= 0)
		{
			return;
		}

		if (Skill4HitEffect)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));

			GetWorld()->SpawnActor<APP_EffectBase>(Skill4HitEffect, HittedMonster->GetActorLocation(),
				FRotator::ZeroRotator, param);
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
		}

		FDamageEvent	DmgEvent;

		float DamageAmount = PlayerInfo.Attack[0] * 2;
		if (isSkill3)
			DamageAmount *= 2;

		float temp_monstr_hp = HittedMonster->TakeDamage(DamageAmount, DmgEvent, GetController(), this);
		if (HittedMonster == TargetedMonster)
			if (temp_monstr_hp < 0)
				TargetFree();
	}
}

void APP_ElfSwordMaster::Skill5Start()
{
	if (!UseMp(400))
	{
		SetIdle();
		return;
	}
}

void APP_ElfSwordMaster::Skill5()
{
	//Super::Skill5();
	//스킬범위 설정 필요함
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill5 Fire")));

	TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), PlayerInfo.Attack_range * 3, GetWorld(), this, ECC_GameTraceChannel3, true);

	for (auto& hitted : temp)
	{
		APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
		if (HittedMonster)
		{
			//시체는 안건드림
			if (HittedMonster->GetStatus()->Hp <= 0)
			{
				//HittedMonster->ChangeAnimState(AnimType::Death);
				continue;
			}

			if (Skill5HitEffect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));

				GetWorld()->SpawnActor<APP_EffectBase>(Skill5HitEffect, HittedMonster->GetActorLocation(),
					FRotator::ZeroRotator, param);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
			}

			FDamageEvent	DmgEvent;

			float DamageAmount = PlayerInfo.Attack[0];
			if (isSkill3)
				DamageAmount *= 2;
			int temphp = HittedMonster->GetStatus()->Hp; //흡혈용


			float temp_monstr_hp = HittedMonster->TakeDamage(DamageAmount * 10, DmgEvent, GetController(), this);

			//타겟을 죽였으면 갱신
			if (TargetedMonster)
			{
				if (HittedMonster == TargetedMonster)
					if (temp_monstr_hp < 0)
						TargetFree();
			}

			//흡혈
			int tempDmg = temphp - temp_monstr_hp;
			if (tempDmg > 0)
			{
				PlayerInfo.Hp += tempDmg;
				if (PlayerInfo.Hp >= PlayerInfo.Max_hp)
					PlayerInfo.Hp = PlayerInfo.Max_hp;

				if (MyPortraitWidget)
					MyPortraitWidget->SetHpPercent(PlayerInfo.Hp * 1.0 / PlayerInfo.Max_hp);

				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill5 HP Absorb HP %d"), PlayerInfo.Hp));
			}
		}
	}
}
void APP_ElfSwordMaster::Death()
{
	Super::Death();
}
void APP_ElfSwordMaster::Reraise()
{
	Super::Reraise();
	ChangeAnimState(AnimType::Reraise);
}
void APP_ElfSwordMaster::Paralysis()
{
	ChangeAnimState(AnimType::Paralysis); SetIdle(); PlayerInfo.Debuff[0] = true;
	Super::Paralysis();
}
void APP_ElfSwordMaster::UsingItem(ConsumeItemType type, APP_Player* target)
{
	Super::UsingItem(type, target);
}
void APP_ElfSwordMaster::SetIdle()
{
	Super::SetIdle();
	SKillTargetFree();
}

float APP_ElfSwordMaster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float fDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (fDamage > 0)
	{
		//스킬2반격대미지 누적
		if (isSkill2)
			SKill2_Damage += fDamage;
		else
			SKill2_Damage = 0;
	}
	return fDamage;
}

void APP_ElfSwordMaster::Skill2Start()
{
	if (!UseMp(100))
	{
		SetIdle();
		return;
	}

	isSkill2 = true;
	SKill2_Damage = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill2 Start")));
}

void APP_ElfSwordMaster::Skill2End()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Skill2 Fire %d"), SKill2_Damage));

	TArray<FHitResult> temp = SphereMulti(this->GetActorLocation(), PlayerInfo.Attack_range * 1.2, GetWorld(), this, ECC_GameTraceChannel3, true);

	for (auto& hitted : temp)
	{
		APP_Monster* HittedMonster = Cast<APP_Monster>(hitted.GetActor());
		if (HittedMonster)
		{
			//시체는 안건드림
			if (HittedMonster->GetStatus()->Hp <= 0)
			{
				//HittedMonster->ChangeAnimState(AnimType::Death);
				continue;
			}

			if (Skill2HitEffect)
			{
				FActorSpawnParameters	param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				//기본공격 이펙트 랜더회전
				FRandomStream Rs;
				Rs.GenerateNewSeed();
				FRotator randRot;
				randRot.Yaw = Rs.FRandRange(0.f, 180);
				randRot.Pitch = Rs.FRandRange(0.f, 180);
				randRot.Roll = Rs.FRandRange(0.f, 180);

				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("rndrot %f %f %f"), randRot.Yaw, randRot.Pitch, randRot.Roll));

				GetWorld()->SpawnActor<APP_EffectBase>(Skill2HitEffect, HittedMonster->GetActorLocation(),
					randRot, param);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("none hit ef")));
			}

			FDamageEvent	DmgEvent;
			float temp_monstr_hp = HittedMonster->TakeDamage(SKill2_Damage * 10, DmgEvent, GetController(), this);
			if (HittedMonster == TargetedMonster)
				if (temp_monstr_hp < 0)
					TargetFree();
		}
	}


	isSkill2 = false;
	SKill2_Damage = 0;
}

void APP_ElfSwordMaster::LevelUP()
{
	Super::LevelUP();
	int i = 0;
	while (PlayerInfo.Exp >= 5000)
	{
		i++;
		PlayerInfo.Exp -= 5000;
	}
	while(i>0)
	{
		i--;
		PlayerInfo.Max_hp += 3000;
		PlayerInfo.Max_mp += 300;
		PlayerInfo.Attack[0] += 1000;
		PlayerInfo.Defense[0] += 600;
		PlayerInfo.Hp = PlayerInfo.Max_hp;
		PlayerInfo.Mp = PlayerInfo.Max_mp;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Level UP")));
	}
}