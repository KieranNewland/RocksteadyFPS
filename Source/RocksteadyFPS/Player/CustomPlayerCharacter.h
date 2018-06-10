// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "CustomPlayerCharacter.generated.h"

UCLASS()
class ROCKSTEADYFPS_API ACustomPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomPlayerCharacter(const FObjectInitializer&);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//Controls
	float GetPitch() { return m_nPitch; }
	float GetYaw() { return m_nYaw; }

	void SetPitch(float nPitch) { m_nPitch = nPitch; }

private:

	UCameraComponent * m_pCamera;

	//Movement
	void WalkForward(float);
	void Strafe(float);

	void BeginCrouch();
	void EndCrouch();

	FVector ActorToWorldDirection(FVector pDirection);

	//Aiming
	void MouseHorizontal(float);
	void MouseVertical(float);

	float m_nPitch;
	float m_nYaw;

	//Shooting TODO - Move to Gun class
	void BeginShooting();
	void EndShooting();

	void SpawnProjectile();

	UPROPERTY(EditDefaultsOnly)
		float m_nShotRange = 100000;

	UFUNCTION(BlueprintCallable)
		bool isShooting() { return m_bShooting; }

	bool m_bShooting;
	float m_nTimeUntilShot;
};
