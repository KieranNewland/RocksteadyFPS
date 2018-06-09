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
	FVector pUpVector = pController->GetControlRotation().Quaternion() * FVector::UpVector;
	FVector pOurPosition = GetActorFeetLocation();
	FVector pStartPosition = pOurPosition + pUpVector * MaxStepHeight;
	FVector pEndPosition = pOurPosition - pUpVector * MaxStepHeight;

	GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::White, FString::Printf(TEXT("Up direction: %f, %f, %f"), pUpVector.X, pUpVector.Y, pUpVector.Z));

	//Sphere trace below the feet for the floor
	FCollisionQueryParams pCollisionParams = FCollisionQueryParams();
	pCollisionParams.AddIgnoredActor(GetOwner());

	FHitResult pOutHit = FHitResult();
	if (pWorld->SweepSingleByChannel(pOutHit, pStartPosition, pEndPosition, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(40), pCollisionParams))
		GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::Green, TEXT("Above actor ") + pOutHit.GetActor()->GetActorLabel());
	else
		GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::Red, TEXT("Didn't hit ground"));

	FVector pNormal = pOutHit.ImpactNormal;

	//Rotate our aim vector by the floor normal
	FRotator pAimRotator = FRotator(-pCustomCharacter->GetPitch(), pCustomCharacter->GetYaw(), 0);
	FQuat pFloorRotation = FQuat::FindBetweenNormals(FVector::UpVector, pNormal);

	//Smooth our rotation to the new desired floor rotation
	FQuat pSmoothFloorRotation = FQuat::Slerp(m_pLastFloorRotation, pFloorRotation, m_nSnapRotationToFloorSpeed * nDelta);
	FQuat pFinalRotation = pSmoothFloorRotation * pAimRotator.Quaternion();
	m_pLastFloorRotation = pSmoothFloorRotation;

	//Set the controller rotation by the new quaternion
	pController->SetControlRotation(pFinalRotation.Rotator());

	GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::White, FString::Printf(TEXT("Normal: %f, %f, %f"), pNormal.X, pNormal.Y, pNormal.Z));
}