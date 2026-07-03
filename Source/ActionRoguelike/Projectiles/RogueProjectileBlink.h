// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileBlink.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueProjectileBlink : public ARogueProjectile
{
	GENERATED_BODY()

protected:
	
	// Re-usable handle for the initial explosion delay and the blink
	FTimerHandle BlinkTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float ExplosionDelay = 0.2f;;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float BlinkDelay = 0.2f;
	
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	void Explode();
	
	void Teleport();
	
public:
	
	ARogueProjectileBlink();
	
	virtual void BeginPlay() override;
};
