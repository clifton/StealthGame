// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	// defaults
	LaunchForce = 700.0f;
	LaunchZBoost = 1.5f;

	MeshComp = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RootComponent = MeshComp;

	CollisionComp = CreateEditorOnlyDefaultSubobject<UBoxComponent>(TEXT("CollissionComp"));
	CollisionComp->SetupAttachment(MeshComp);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchpadCollision);
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();

	FVector BoxSize = CollisionComp->GetScaledBoxExtent();
	FVector BoxLocation = CollisionComp->GetComponentLocation();

	ImpulseSource.X = BoxLocation.X - BoxSize.X / 2;
	ImpulseSource.Y = BoxLocation.Y;
	ImpulseSource.Z = BoxLocation.Z - BoxSize.Z / 2;

	ImpulseRotationVector = CollisionComp->GetComponentRotation().Vector();
}

void AFPSLaunchPad::OverlapLaunchpadCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapLaunchpadCollision triggered!"));

	FVector OtherLocation = OtherComp->GetComponentLocation();

	UE_LOG(LogTemp, Warning, TEXT("Impulse Source: %s"), *ImpulseSource.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Other Location: %s"), *OtherLocation.ToString());

	// launchforce * rotation + ZBoost
	FVector const LaunchVelocity = FVector(LaunchForce) * ImpulseRotationVector + FVector(0.0f, 0.0f, LaunchZBoost * LaunchForce);

	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if (Character) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor used launchpad"));
		Character->LaunchCharacter(LaunchVelocity, true, true);
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("Component used launchpad"));
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
	}
	else
	{
		return;
	}
	PlayEffects();
}

void AFPSLaunchPad::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
}