// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "CustomCharacterMovementComponent.h"
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

private:

	//Movement
	UPROPERTY(VisibleAnywhere)
		UCharacterMovementComponent * m_pMovementComponent;
	void WalkForward(float);
	void Strafe(float);

	//Mouse
	void MouseHorizontal(float);
	void MouseVertical(float);
	void Fire();
	
};
