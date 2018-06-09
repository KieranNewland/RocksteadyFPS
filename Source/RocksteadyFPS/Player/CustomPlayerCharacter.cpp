// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerCharacter.h"
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
	m_nPitch += nMouseStrength;
}

void ACustomPlayerCharacter::SpawnProjectile(FVector pSpawnPosition, FVector pSpawnDirection)
{
	UWorld* pWorld = GetWorld();

	FCollisionQueryParams pTraceParams = FCollisionQueryParams();
	pTraceParams.AddIgnoredActor(this);
	pTraceParams.TraceTag = TraceTag;

	FHitResult pHitResult = FHitResult();
	if( pWorld->LineTraceSingleByChannel(pHitResult, pSpawnPosition, pSpawnPosition + pSpawnDirection * m_nShotRange, ECC_GameTraceChannel1, pTraceParams))
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Hit target: ") + pHitResult.GetActor()->GetActorLabel());
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Did not hit target"));
}