// Fill out your copyright notice in the Description page of Project Settings.


#include "PP_ArachBoss.h"
#include "KTWFunc.h"


APP_ArachBoss::APP_ArachBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//메시초기화
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Arachnid_Boss/Mesh/SK_Arachnid_Boss.SK_Arachnid_Boss'"));
	if (MeshAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.07f, 0.1f, 0.3f));
	GetMesh()->AddRelativeLocation(FVector(0, 0, -82.f));
	
	//애니메이션초기화
	static ConstructorHelpers::FClassFinder<UAnimInstance>	AnimAsset(TEXT("AnimBlueprint'/Game/0_PP/0_Monsters/ArachBoss/BP_PPArachBossAnim.BP_PPArachBossAnim_C'"));
	if (AnimAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
		

}

// Called when the game starts or when spawned
void APP_ArachBoss::BeginPlay()
{
	Super::BeginPlay();

	MyAnim = Cast<UPP_ArachBossAnim>(GetMesh()->GetAnimInstance());

}

// Called every frame
void APP_ArachBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	FHitResult temp = DetectSphere(this->GetActorLocation(), 3000.f, GetWorld(), this, ECC_GameTraceChannel4);
	m_Target = Cast<APawn>(temp.GetActor());
	if (m_Target)
	{
		float dist = FVector::Distance(m_Target->GetActorLocation(), GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Dist : %f"), dist));
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
	}
	else
	{
		GetController()->StopMovement();
	}
	*/

	/*
	FHitResult temp = DetectSphere(this->GetActorLocation(), 10000.f, GetWorld(), this, ECC_GameTraceChannel4);
	m_Target = Cast<ACharacter>(temp.GetActor());
	if (m_Target)
	{

		float dist = FVector::Distance(m_Target->GetActorLocation(), GetActorLocation());
		if (dist > 650.f)
		{
			if (MyAnim)
				MyAnim->MyAnimType = AnimType::Move;
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_Target);
		}
		else
		{
			GetController()->StopMovement();
			if (MyAnim)
			{
				if (MyAnim->MyAnimType == AnimType::Move)
					MyAnim->MyAnimType = AnimType::Skill1;
				else if (MyAnim->MyAnimType == AnimType::Idle)
					MyAnim->MyAnimType = AnimType::Attack1;
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("none target")));
		GetController()->StopMovement();
		if (MyAnim)
			MyAnim->MyAnimType = AnimType::Idle;
	}
	*/
}

// Called to bind functionality to input
void APP_ArachBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}