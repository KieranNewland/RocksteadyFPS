// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomPlayerCharacter.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROCKSTEADYFPS_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	void InitializeComponent() override;

	FQuat GetLastFloorRotation() { return m_pLastFloorRotation; }

private:
	UPROPERTY(EditDefaultsOnly)
		float m_nSnapRotationToFloorSpeed = 0.3f;

		FQuat m_pLastFloorRotation;

		void OnMovementUpdated(float, const FVector&, const FVector&) override;
	
		void RotateCharacterToFollowFloor(float);
};
