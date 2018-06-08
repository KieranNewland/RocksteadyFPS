// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROCKSTEADYFPS_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
		void OnMovementUpdated(float, const FVector&, const FVector&) override;
	
	
};
