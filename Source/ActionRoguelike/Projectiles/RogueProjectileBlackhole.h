// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileBlackhole.generated.h"

class URadialForceComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueProjectileBlackhole : public ARogueProjectile
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Blackhole")
	TObjectPtr<UNiagaraSystem> BlackholeEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Component")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Blackhole")
	float BlackholeRadius = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Blackhole")
	float BlackholeForce = -700000.f;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	void DestroyTimerElapsed();

public:
	
	virtual void PostInitializeComponents() override;
	
	ARogueProjectileBlackhole();
	
};
