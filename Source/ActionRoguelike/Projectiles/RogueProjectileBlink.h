// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileBlink.generated.h"

class UAudioComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueProjectileBlink : public ARogueProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
		
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UAudioComponent>  LoopedAudioComponent;
	
	FTimerHandle ExplodeTimerHandle;
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void Explode();
	
	void Teleport();
	
	void DestroyTimerElapsed();
	
public:
	
	virtual void PostInitializeComponents() override;
	
	ARogueProjectileBlink();
};
