// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomCharacterMovementComponent.h"

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

	//Make sure we're checking below the rotated feet
	const FVector pUpVector = pController->GetControlRotation().Quaternion() * FVector::UpVector;
	const FVector pOurPosition = GetActorFeetLocation();
	const FVector pStartPosition = pOurPosition + pUpVector * MaxStepHeight;
	const FVector pEndPosition = pOurPosition - pUpVector * MaxStepHeight;

	//Sphere trace below the feet for the floor
	FCollisionQueryParams pCollisionParams = FCollisionQueryParams();
	pCollisionParams.AddIgnoredActor(GetOwner());

	FHitResult pOutHit = FHitResult();
	if (pWorld->SweepSingleByChannel(pOutHit, pStartPosition, pEndPosition, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(30), pCollisionParams))
		GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::Green, TEXT("Above actor ") + pOutHit.GetActor()->GetActorLabel());
	else
		GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::Red, TEXT("Didn't hit ground"));

	const FVector pNormal = pOutHit.ImpactNormal;

	//Smooth our current floor rotation to the new desired floor rotation
	const FQuat pDesiredFloorRotation = FQuat::FindBetweenNormals(FVector::UpVector, pNormal);
	const FQuat pSmoothFloorRotation = FQuat::Slerp(m_pLastFloorRotation, pDesiredFloorRotation, m_nSnapRotationToFloorSpeed * nDelta);

	//Rotate our aim vector by the smoothed floor rotation - save the rotation so we can smooth next frame
	const FRotator pAimRotator = FRotator(-pCustomCharacter->GetPitch(), pCustomCharacter->GetYaw(), 0);
	const FQuat pFinalRotation = pSmoothFloorRotation * pAimRotator.Quaternion();
	m_pLastFloorRotation = pSmoothFloorRotation;

	//Set the controller rotation by the new quaternion
	pController->SetControlRotation(pFinalRotation.Rotator());
}