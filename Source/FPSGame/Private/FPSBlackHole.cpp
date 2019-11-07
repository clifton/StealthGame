// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	BlackHoleCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("BlackHoleCollisionComp"));
	BlackHoleCollisionComp->SetupAttachment(MeshComp);

	BlackHoleCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapBlackHoleCollision);

	BlackHoleForceComp = CreateDefaultSubobject<USphereComponent>(TEXT("BlackHoleForceComp"));
	BlackHoleForceComp->SetupAttachment(MeshComp);
}

void AFPSBlackHole::OverlapBlackHoleCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSBlackHole::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	BlackHoleForceComp->GetOverlappingComponents(OverlappingComps);

	for (auto It = OverlappingComps.CreateIterator(); It; It++)
	{
		UPrimitiveComponent* Comp = *It;
		if (Comp && Comp->IsSimulatingPhysics())
		{
			Comp->AddRadialForce(GetActorLocation(), BlackHoleForceComp->GetScaledSphereRadius(), BlackHoleForce, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

