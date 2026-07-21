// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


ARoguePickup::ARoguePickup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->SetSphereRadius(SphereRadius);
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName("Pickup");
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	MeshComponent->SetupAttachment(SphereComponent);
}

void ARoguePickup::OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator);
	
	OnPickup(OtherActor);
	
	Destroy();
}

void ARoguePickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlappedActor);
}

void ARoguePickup::OnPickup(AActor* OverlappedActor)
{
}

