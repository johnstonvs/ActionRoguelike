// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


ARoguePickup::ARoguePickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->SetCollisionProfileName("OverlapOnlyPawn");
	SphereComponent->SetSphereRadius(SphereRadius);
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName("Pickup");
}

void ARoguePickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlappedActor);
}

void ARoguePickup::OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Add functionality on child pickup actor
}