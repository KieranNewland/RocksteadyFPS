// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomCharacterMovementComponent.h"

void UCustomCharacterMovementComponent::OnMovementUpdated(float nDelta, const FVector& pOldLocation, const FVector& pOldVelocity)
{
	Super::OnMovementUpdated(nDelta, pOldLocation, pOldVelocity);

	//Trace below our new position to check the normal of the floor
	UWorld* pWorld = GetWorld();

	check(pWorld != nullptr);

	FVector pUpVector = UpdatedComponent->GetUpVector();
	FVector pOurPosition = GetActorFeetLocation();
	FVector pStartPosition = pOurPosition + pUpVector * MaxStepHeight;
	FVector pEndPosition = pOurPosition - pUpVector * MaxStepHeight;

	FCollisionQueryParams pCollisionParams = FCollisionQueryParams();
	pCollisionParams.AddIgnoredActor(GetOwner());

	FHitResult pOutHit = FHitResult();
	if (pWorld->LineTraceSingleByChannel(pOutHit, pOurPosition, pEndPosition, ECC_GameTraceChannel1, pCollisionParams))
		GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::Green, TEXT("Above actor ") + pOutHit.GetActor()->GetActorLabel());
	else
		GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::Red, TEXT("Didn't hit ground"));

	FVector pNormal = pOutHit.ImpactNormal;
	/*FQuat pRotator = FQuat::FindBetween(GetCharacterOwner()->GetActorUpVector(), pNormal);
	GetCharacterOwner()->AddActorWorldRotation(pRotator);*/
	GEngine->AddOnScreenDebugMessage(-1, nDelta, FColor::White, FString::Printf(TEXT("Normal: %f, %f, %f"), pNormal.X, pNormal.Y, pNormal.Z));
}