// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerCharacter.h"

ACustomPlayerCharacter::ACustomPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACustomPlayerCharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	pPlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACustomPlayerCharacter::Fire);
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
	AddControllerYawInput(nMouseStrength);
}

void ACustomPlayerCharacter::MouseVertical(float nMouseStrength)
{
	AddControllerPitchInput(nMouseStrength);
}

void ACustomPlayerCharacter::Fire()
{

}