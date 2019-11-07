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

	BlackHoleForceComp = CreateDefaultSubobject<USphereComponent>(TEXT("BlackHoleForceComp"));
	BlackHoleForceComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlackHoleForceComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BlackHoleForceComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	BlackHoleForceComp->SetupAttachment(MeshComp);

	BlackHoleCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("BlackHoleCollisionComp"));
	BlackHoleCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlackHoleCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BlackHoleCollisionComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	BlackHoleCollisionComp->SetupAttachment(MeshComp);
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

	TArray<AActor*> CollectedActors;
	BlackHoleForceComp->GetOverlappingActors(CollectedActors);
	for (auto It = CollectedActors.CreateIterator(); It; It++)
	{
		UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*It)->GetRootComponent());
		if (SM)
		{
			SM->AddRadialForce(GetActorLocation(), BlackHoleForceComp->GetScaledSphereRadius(), BlackHoleForce, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

	BlackHoleCollisionComp->GetOverlappingActors(CollectedActors);
	for (auto It = CollectedActors.CreateIterator(); It; It++)
	{
		UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*It)->GetRootComponent());
		if (SM)
		{
			SM->DestroyComponent();
		}
	}
}

