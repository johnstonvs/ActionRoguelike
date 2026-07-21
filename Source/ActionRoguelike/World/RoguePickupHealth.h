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
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> PickupMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> PickupSound;
	
	UPROPERTY(EditDefaultsOnly, Category="PickupHealth")
	float HealthToGive = 50.f;
	
	virtual void OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

public:
	ARoguePickupHealth();
};
