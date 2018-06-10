// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomCharacterMovementComponent.h"

void UCustomCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetWalkableFloorAngle(90);
	bMaintainHorizontalGroundVelocity = false;
}

void UCustomCharacterMovementComponent::OnMovementUpdated(float nDelta, const FVector& pOldLocation, const FVector& pOldVelocity)
{
	Super::OnMovementUpdated(nDelta, pOldLocation, pOldVelocity);

	RotateCharacterToFollowFloor(nDelta);
}

void UCustomCharacterMovementComponent::RotateCharacterToFollowFloor(float nDelta)
{
	UWorld* pWorld = GetWorld();

	check(pWorld != nullptr);

	ACustomPlayerCharacter* pCustomCharacter = (ACustomPlayerCharacter*)GetCharacterOwner();
	AController* pController = pCustomCharacter->Controller;

	//Get the normal of the floor below our feet
	const FVector pNormal = CurrentFloor.HitResult.Normal;
	GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::White, FString::Printf(TEXT("Normal: %f, %f, %f"), pNormal.X, pNormal.Y, pNormal.Z));

	//Smooth our current floor rotation to the new desired floor rotation
	const FQuat pDesiredFloorRotation = FQuat::FindBetweenNormals(FVector::UpVector, pNormal);
	const FQuat pSmoothFloorRotation = FQuat::Slerp(m_pLastFloorRotation, pDesiredFloorRotation, m_nSnapRotationToFloorSpeed * nDelta);

	//Clamp the pitch -  if pitch goes too far we think we're walking in the opposite direction as it takes from the horizontal direction
	const FRotator pSmoothFloorRotator = pSmoothFloorRotation.Rotator();
	float nFloorPitch = pSmoothFloorRotator.Pitch;
	float nPlayerPitch = pCustomCharacter->GetPitch();

	//Rotate our aim vector by the smoothed floor rotation - save the rotation so we can smooth next frame
	const FRotator pAimRotator = FRotator(-nPlayerPitch, pCustomCharacter->GetYaw(), 0);
	const FQuat pFinalRotation = pSmoothFloorRotation * pAimRotator.Quaternion();

	//GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::Green, FString::Printf(TEXT("Final: %f, %f Pitch: %f, %f"), pFinalEuler.X, pFinalEuler.Y, nPlayerPitch, nFloorPitch));

	m_pLastFloorRotation = pSmoothFloorRotation;

	//Set the controller rotation by the new quaternion
	pController->SetControlRotation(pFinalRotation.Rotator());
}