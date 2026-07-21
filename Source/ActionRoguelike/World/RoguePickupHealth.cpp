// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupHealth.h"

#include "ActionSystem/RogueActionSystemComponent.h"
#include "Player/RoguePlayerCharacter.h"


// Sets default values
ARoguePickupHealth::ARoguePickupHealth()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoguePickupHealth::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoguePickupHealth::OnPickup(AActor* OverlappedActor)
{
	Super::OnPickup(OverlappedActor);
	
	if (ARoguePlayerCharacter* Player = Cast<ARoguePlayerCharacter>(OverlappedActor))
	{
		Player->GetActionSystemComponent()->ApplyHealthChange(HealthToGive);
	}
}

