// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	MeshComp = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	RootComponent = MeshComp;

	CollisionComp = CreateEditorOnlyDefaultSubobject<UBoxComponent>(TEXT("CollissionComp"));
	CollisionComp->SetupAttachment(MeshComp);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchpadCollision);
}

void AFPSLaunchPad::OverlapLaunchpadCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapLaunchpadCollision triggered!"));
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
}
