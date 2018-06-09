// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerCharacter.h"
#include "../Enemy/EnemyCharacter.h"
#include "CustomCharacterMovementComponent.h"

const FName TraceTag("MyTraceTag");

ACustomPlayerCharacter::ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACustomPlayerCharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->DebugDrawTraceTag = TraceTag;
}

// Called every frame
void ACustomPlayerCharacter::Tick(float nDeltaTime)
{
	Super::Tick(nDeltaTime);

	if (!m_bShooting)
		return;

	m_nTimeUntilShot -= nDeltaTime;

	if (m_nTimeUntilShot > 0)
		return;

	SpawnProjectile();
	m_nTimeUntilShot = 0.1f;
}

void ACustomPlayerCharacter::SpawnProjectile()
{
	UWorld* pWorld = GetWorld();
	UCameraComponent* pCamera = FindComponentByClass<UCameraComponent>();

	check(pCamera != nullptr);

	FCollisionQueryParams pTraceParams = FCollisionQueryParams();
	pTraceParams.AddIgnoredActor(this);
	pTraceParams.TraceTag = TraceTag;

	FHitResult pHitResult = FHitResult();
	if (!pWorld->LineTraceSingleByChannel(pHitResult, pCamera->GetComponentLocation(), pCamera->GetComponentLocation() + pCamera->GetForwardVector() * m_nShotRange, ECC_GameTraceChannel1, pTraceParams))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Did not hit target"));
		return;
	}

	AActor* pActor = pHitResult.GetActor();

	if (!pActor->IsA<AEnemyCharacter>())
		return;

	AEnemyCharacter* pEnemy = (AEnemyCharacter*)pActor;
	pEnemy->InflictDamage(1);

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Hit target: ") + pHitResult.GetActor()->GetActorLabel());
}

// Called to bind functionality to input
void ACustomPlayerCharacter::SetupPlayerInputComponent(UInputComponent* pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(pPlayerInputComponent);

	pPlayerInputComponent->BindAxis("MoveForward", this, &ACustomPlayerCharacter::WalkForward);
	pPlayerInputComponent->BindAxis("MoveRight", this, &ACustomPlayerCharacter::Strafe);

	pPlayerInputComponent->BindAxis("Turn", this, &ACustomPlayerCharacter::MouseHorizontal);
	pPlayerInputComponent->BindAxis("LookUp", this, &ACustomPlayerCharacter::MouseVertical);

	pPlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	pPlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	pPlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACustomPlayerCharacter::BeginShooting);
	pPlayerInputComponent->BindAction("Fire", IE_Released, this, &ACustomPlayerCharacter::EndShooting);

	pPlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACustomPlayerCharacter::BeginCrouch);
	pPlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACustomPlayerCharacter::EndCrouch);
}

void ACustomPlayerCharacter::WalkForward(float nWalkStrength)
{
	AddMovementInput(GetActorForwardVector(), nWalkStrength);
}

void ACustomPlayerCharacter::Strafe(float nWalkStrength)
{
	AddMovementInput(GetActorRightVector(), nWalkStrength);
}

void ACustomPlayerCharacter::MouseHorizontal(float nMouseStrength)
{
	m_nYaw += nMouseStrength;
}

void ACustomPlayerCharacter::MouseVertical(float nMouseStrength)
{
	m_nPitch = FMath::Clamp<float>(m_nPitch + nMouseStrength, -80, 80);
}

void ACustomPlayerCharacter::BeginShooting()
{
	m_bShooting = true;
	m_nTimeUntilShot = 0;
}

void ACustomPlayerCharacter::EndShooting()
{
	m_bShooting = false;
}

void ACustomPlayerCharacter::BeginCrouch()
{
	Crouch(true);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("Crouching"));
}

void ACustomPlayerCharacter::EndCrouch()
{
	UnCrouch(true);
}