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
	
public:

	void BeginPlay() override;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<ATargetPoint*> Waypoints;
	
	void GoToWaypoint(ATargetPoint Waypoint);
};
