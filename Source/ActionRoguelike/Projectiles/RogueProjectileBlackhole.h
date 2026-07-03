// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileBlackhole.generated.h"

class UAudioComponent;
class URadialForceComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueProjectileBlackhole : public ARogueProjectile
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> BlackholeEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> BlackholeSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Component")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> ActiveBlackholeSoundComp = nullptr;
	
	UFUNCTION()
	void OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void HandleDestroyed(AActor* DestroyedActor);

public:
	
	virtual void PostInitializeComponents() override;
	
	ARogueProjectileBlackhole();
	
};
