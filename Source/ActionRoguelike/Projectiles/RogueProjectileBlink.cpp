// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBlink.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARogueProjectileBlink::ARogueProjectileBlink()
{
	
	SphereComponent->SetCollisionProfileName("Projectile");
	
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);
	
	LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComp"));
	LoopedAudioComponent->SetupAttachment(SphereComponent);
	
}

void ARogueProjectileBlink::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileBlink::OnActorHit);
	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	
	// Explode after 0.2 seconds
	const float ExplodeDelayTime = 0.2f;
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ARogueProjectileBlink::Explode, ExplodeDelayTime);
}


void ARogueProjectileBlink::Explode()
{
	
	ProjectileMovementComponent->StopMovementImmediately();
	LoopedAudioComponent->Deactivate();
	LoopedNiagaraComponent->Deactivate();
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(ExplosionEffect, SphereComponent, NAME_None,
    		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	// Teleport instigator after 0.2 seconds
	FTimerHandle TeleportTimerHandle;
	const float TeleportDelayTime = 0.2f;
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ARogueProjectileBlink::Teleport, TeleportDelayTime);
	
}

void ARogueProjectileBlink::Teleport()
{
	TObjectPtr<APawn> TeleportTarget = GetInstigator();
	
	if (TeleportTarget)
	{
		FRotator TeleportRotation(0.f, GetActorRotation().Yaw, 0.f);
		GetInstigator()->TeleportTo(GetActorLocation(), TeleportRotation);
	}
	
	// Destroy after 0.5 seconds;
	FTimerHandle DestroyTimerHandle;
	const float  DestroyDelayTime = 0.5f;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ARogueProjectileBlink::DestroyTimerElapsed, DestroyDelayTime);
	
}

void ARogueProjectileBlink::DestroyTimerElapsed()
{
	Destroy();
}

void ARogueProjectileBlink::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
	Explode();
	
}


