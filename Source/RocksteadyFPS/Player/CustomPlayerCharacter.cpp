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
	
	//Use the PlayerStart rotation
	m_nYaw = GetActorRotation().Euler().Z;
	GetWorld()->DebugDrawTraceTag = TraceTag;

	m_pCamera = FindComponentByClass<UCameraComponent>();
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
		return;

	AActor* pActor = pHitResult.GetActor();

	if (!pActor->IsA<AEnemyCharacter>())
		return;

	AEnemyCharacter* pEnemy = (AEnemyCharacter*)pActor;
	pEnemy->InflictDamage(1);
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
	FVector pActorForward = ActorToWorldDirection(m_pCamera->GetForwardVector());

	AddMovementInput(pActorForward, nWalkStrength);
}

void ACustomPlayerCharacter::Strafe(float nWalkStrength)
{
	FVector pActorRight = ActorToWorldDirection(m_pCamera->GetRightVector());

	AddMovementInput(pActorRight, nWalkStrength);
}

FVector ACustomPlayerCharacter::ActorToWorldDirection(FVector pDirection)
{
	//We need to convert the direction from actor coordinates to consider the normal of the floor
	UCustomCharacterMovementComponent* pMovementComponent = (UCustomCharacterMovementComponent*)GetCharacterMovement();
	FVector pLastFloorRotation = pMovementComponent->GetLastFloorRotation() * FVector::UpVector;
	FQuat pDifference = FQuat::FindBetweenNormals(pLastFloorRotation, FVector::UpVector);

	return pDifference * pDirection;
}

void ACustomPlayerCharacter::MouseHorizontal(float nMouseStrength)
{
	m_nYaw += nMouseStrength;
}

void ACustomPlayerCharacter::MouseVertical(float nMouseStrength)
{
	m_nPitch += nMouseStrength;
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