// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	m_nCurrentHealth = m_nMaxHealth;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::InflictDamage(float nDamage)
{
	m_nCurrentHealth -= nDamage;

	GEngine->AddOnScreenDebugMessage(-1, 0.3f, FColor::White, FString::Printf(TEXT("Hit enemy %f"), m_nCurrentHealth));

	if (m_nCurrentHealth <= 0)
		Kill();
}

void AEnemyCharacter::Kill()
{
	SetActorHiddenInGame(true);
}