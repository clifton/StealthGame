// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIGuardController.generated.h"

class ATargetPoint;

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIGuardController : public AAIController
{
	GENERATED_BODY()

private:
	uint8 INextTargetPoint;
	
protected:

	UPROPERTY()
	TArray<AActor*> Waypoints;

	UPROPERTY()
	ATargetPoint* NextTargetPoint;

public:

	void BeginPlay() override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
	void GoToNextWaypoint();
	
};
