// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupHealth.h"

#include "ActionSystem/RogueActionSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARoguePickupHealth::ARoguePickupHealth()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	PickupMeshComponent->SetCollisionProfileName("NoCollision");
	PickupMeshComponent->SetupAttachment(RootComponent);
}

void ARoguePickupHealth::OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Get the other actors' action component
	URogueActionSystemComponent* ActionComp = OtherActor->GetComponentByClass<URogueActionSystemComponent>();
	
	// Assert if null, then we misconfigured what we can overlap with, any Pawn should have an action component
	// Skip pickup if already at full health
	if (ensure(ActionComp != nullptr) && !ActionComp->IsFullHealth())
	{
		ActionComp->ApplyHealthChange(HealthToGive);
		
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator);
	
		Destroy();
	}
}

