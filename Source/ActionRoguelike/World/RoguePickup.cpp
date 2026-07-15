// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


ARoguePickup::ARoguePickup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
}


void ARoguePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoguePickup::OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ARoguePickup::HandleOverlap(AActor* OverlappedActor)
{
}

