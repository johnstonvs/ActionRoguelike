// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBlink.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARogueProjectileBlink::ARogueProjectileBlink()
{
	ProjectileMovementComponent->InitialSpeed = 6000.f;
}

void ARogueProjectileBlink::BeginPlay()
{
	Super::BeginPlay();
	
	// Explode after delay
	GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ThisClass::Explode, ExplosionDelay);
}


void ARogueProjectileBlink::Explode()
{
	PlayExplosionEffects();
	
	ProjectileMovementComponent->StopMovementImmediately();
	LoopedAudioComponent->Deactivate();
	LoopedNiagaraComponent->Deactivate();
	
	// Blink after delay
	GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ThisClass::Teleport, BlinkDelay);
	
}

void ARogueProjectileBlink::Teleport()
{
	TObjectPtr<APawn> ActorToTeleport = GetInstigator();
	// TODO: check if instigator is still alive before teleporting
	check(ActorToTeleport);
	
	ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
	
	// Clear projectile from world now that timers are no longer running
	Destroy();
}

void ARogueProjectileBlink::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Skip parent class implementation as to not destroy yet	
	
	GetWorldTimerManager().ClearTimer(BlinkTimerHandle);
	Explode();
}


