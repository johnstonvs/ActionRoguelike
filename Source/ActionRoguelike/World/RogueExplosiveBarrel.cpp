// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueExplosiveBarrel.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ARogueExplosiveBarrel::ARogueExplosiveBarrel()
{
	SetCanBeDamaged(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(MeshComponent);
	RadialForceComponent->ImpulseStrength = 150000.f;
	RadialForceComponent->Radius = 750.f;
	// Deactivate auto activation to not cause a continuous impulse
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;
}

float ARogueExplosiveBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// Allow exploding once, skip when timer is already running
	if (bExploded || GetWorldTimerManager().TimerExists(ExplosionTimerHandle))
	{
		return ActualDamage;
	}
	
	// Hold onto both Niagara and Audio Components to disable during Explode()
		
	ActiveBurningEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BurningEffect, MeshComponent, NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);

	ActiveBurningSoundComp = UGameplayStatics::SpawnSoundAttached(BurningSound, MeshComponent);
	
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ARogueExplosiveBarrel::Explode, ExplosionDelay);

	return ActualDamage;
	
}


void ARogueExplosiveBarrel::Explode()
{
	bExploded = true;
	
	ActiveBurningEffectComp->Deactivate();
	ActiveBurningSoundComp->Stop();

	RadialForceComponent->FireImpulse();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
}

