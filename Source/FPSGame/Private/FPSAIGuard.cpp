// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "FPSGameMode.h"
#include "FPSAIGuardController.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GuardState = EAIState::Idle;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	SetGuardState(EAIState::Alerted);

	AFPSAIGuardController* GuardController = Cast<AFPSAIGuardController>(GetController());
	GuardController->StopMovement();

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->CompleteMission(SeenPawn, false);
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted) return;

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Roll = 0.0f;
	NewLookAt.Pitch = 0.0f;

	SetActorRotation(NewLookAt);
	SetGuardState(EAIState::Suspicious);

	AFPSAIGuardController* GuardController = Cast<AFPSAIGuardController>(GetController());
	GuardController->StopMovement();
	
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted) return;

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);

	AFPSAIGuardController* GuardController = Cast<AFPSAIGuardController>(GetController());
	GuardController->GoToNextWaypoint();
}

// only runs on server
void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState) {
		return;
	}
	GuardState = NewState;

	
	OnRep_GuardState();
}

// only runs on clients
void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}


void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
