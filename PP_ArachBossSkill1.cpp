// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ArachBossSkill1.h"
#include "DestructibleComponent.h"
#include "Components/DecalComponent.h"
#include "PP_Player.h"
#include "KTWFunc.h"

// Sets default values
APP_ArachBossSkill1::APP_ArachBossSkill1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//변수들 초기화
	My_Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	My_RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	My_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	My_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	My_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	SetRootComponent(My_RootScene);
	My_Mesh->SetupAttachment(My_RootScene);
	My_Niagara->SetupAttachment(My_RootScene);
	My_Decal->SetupAttachment(My_RootScene);
	My_Particle->SetupAttachment(My_RootScene);
	MeshBreakTime = 1.0f;

	//메시설정
	/*
	//캐스트실패
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("DestructibleMesh'/Game/0_PP/0_Monsters/ArachBoss/Skill1Static_DM.Skill1Static_DM'"));
	if (MeshAsset.Succeeded())
	{
		USkeletalMesh* sktemp = MeshAsset.Object;
		UDestructibleMesh* temp = Cast<UDestructibleMesh>(sktemp);
	}
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> MeshAsset(TEXT("DestructibleMesh'/Game/0_PP/0_Monsters/ArachBoss/Skill1Static_DM.Skill1Static_DM'"));
	if (MeshAsset.Succeeded())
		My_Mesh->SetDestructibleMesh(MeshAsset.Object);
	*/	
	My_Mesh->SetRelativeScale3D(FVector(3, 3, 3));
	

	//나이아가라설정
	/*
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>	NaiagaraAsset(TEXT("NiagaraSystem'/Game/0_PP/0_Monsters/ArachBoss/NS_SwordAura_3.NS_SwordAura_3'"));
	if (NaiagaraAsset.Succeeded())
		My_Niagara->SetAsset(NaiagaraAsset.Object);
		*/
	My_Niagara->SetRelativeScale3D(FVector(5, 100, 1));
	My_Niagara->SetRelativeLocation(FVector(0, 0, 50));
	
	
	//데칼설정
	static ConstructorHelpers::FObjectFinder<UMaterial>	DecalAsset(TEXT("Material'/Game/0_PP/0_Decal/MTSevarogSkill3Decal.MTSevarogSkill3Decal'"));
	if (DecalAsset.Succeeded())
		My_Decal->SetDecalMaterial(Cast<UMaterialInterface>(DecalAsset.Object));
	My_Decal->DecalSize = FVector(256, 256, 256);
	My_Decal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	
}

// Called when the game starts or when spawned
void APP_ArachBossSkill1::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f);
	My_Niagara->SetVisibility(false);
	My_Decal->SetVisibility(false);
	My_Particle->SetActive(false);
	BeforeBreak = true;
	TickDmgAcc = 0;

	if (My_Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), My_Sound, GetActorLocation());

	My_Mesh->SetRelativeLocation(FVector(My_Mesh->GetRelativeLocation().X, My_Mesh->GetRelativeLocation().Y, My_Mesh->GetRelativeLocation().Z - 200));
}

// Called every frame
void APP_ArachBossSkill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BeforeBreak)
	{
		MeshBreakTime -= DeltaTime;
		if(My_Mesh->GetRelativeLocation().Z + (DeltaTime * 2000) <= 30.f)
			My_Mesh->SetRelativeLocation(FVector(My_Mesh->GetRelativeLocation().X, My_Mesh->GetRelativeLocation().Y, My_Mesh->GetRelativeLocation().Z + (DeltaTime * 2000)));
	}

	if (MeshBreakTime < 0 && BeforeBreak)
	{

		BeforeBreak = false;
		if(vecter1 != 0)
			My_Mesh->ApplyRadiusDamage(vecter1, Super::GetActorLocation(), vecter2, vecter3, true);
		else
			My_Mesh->ApplyRadiusDamage(1000.f, Super::GetActorLocation(), 1000.f, 1000.f, true);
		My_Decal->SetVisibility(true);
		My_Niagara->SetVisibility(true);
		My_Particle->SetActive(true, true);
	}

	if (!BeforeBreak)
	{
		TickDmgAcc += DeltaTime;
		if (TickDmgAcc >= 0.35f)
		{
			TickDmgAcc = 0;
			TArray<FHitResult> temp = SphereMulti(GetActorLocation(), 200, GetWorld(), MyOwner, ECC_GameTraceChannel4);
			for (auto& hitted : temp)
			{
				APP_Player* HittedMonster = Cast<APP_Player>(hitted.GetActor());
				if (HittedMonster)
				{
					//시체는 안건드림
					if (HittedMonster->GetPlayerInfo()->Hp <= 0)
					{
						//HittedMonster->ChangeAnimState(AnimType::Death);
						continue;
					}
					FDamageEvent	DmgEvent;
					HittedMonster->SetPoison();
					float temp_monstr_hp = HittedMonster->TakeDamage(500, DmgEvent, UGameplayStatics::GetPlayerController(GetWorld(), 0), MyOwner);
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Tick damage")));
				}
			}
		}
	}
}

void APP_ArachBossSkill1::SetOwner(APP_Monster* owner)
{
	MyOwner = owner;
}

