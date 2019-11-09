// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuardController.h"
#include "Engine/Targetpoint.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"


void AFPSAIGuardController::BeginPlay()
{
	Super::BeginPlay();

	if (GetPawn()->ActorHasTag(TEXT("NavigatingGuard")))
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);

		if (Waypoints.Num() > 0) {
			INextTargetPoint = 0;
			NextTargetPoint = Cast<ATargetPoint>(Waypoints[INextTargetPoint]);
			GoToNextWaypoint();
		}
	}
}

void AFPSAIGuardController::GoToNextWaypoint()
{
	if (Waypoints.Num() == 0) return;

	UE_LOG(LogTemp, Warning, TEXT("Go to waypoint %d"), INextTargetPoint);

	MoveToActor(NextTargetPoint);
}

void AFPSAIGuardController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess()) {
		if (++INextTargetPoint >= Waypoints.Num()) INextTargetPoint = 0;

		NextTargetPoint = Cast<ATargetPoint>(Waypoints[INextTargetPoint]);
		GoToNextWaypoint();
	}
}

