// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePickup.h"
#include "RoguePickupHealth.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARoguePickupHealth : public ARoguePickup
{
	GENERATED_BODY()


protected:
	
	UPROPERTY(EditDefaultsOnly, Category="PickupHealth")
	float HealthToGive = 20.f;
	
	virtual void BeginPlay() override;
	
	virtual void OnPickup(AActor* OverlappedActor) override;

public:
	ARoguePickupHealth();
};
