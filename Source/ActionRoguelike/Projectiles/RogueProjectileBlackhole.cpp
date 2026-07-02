// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBlackhole.h"

#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


ARogueProjectileBlackhole::ARogueProjectileBlackhole()
{
	
	SphereComponent->SetCollisionProfileName("ProjectileOverlap");
	
	ProjectileMovementComponent->InitialSpeed = 550.f;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(SphereComponent);
	RadialForceComponent->bIgnoreOwningActor = true;
	RadialForceComponent->Radius = BlackholeRadius;
	RadialForceComponent->ForceStrength = BlackholeForce;
	RadialForceComponent->Activate();
}

void ARogueProjectileBlackhole::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueProjectileBlackhole::OnActorOverlap);
	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(BlackholeEffect, SphereComponent, NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	ActiveBlackholeSoundComp = UGameplayStatics::SpawnSoundAttached(BlackholeSound, SphereComponent);
	
	// Destroy self after 5 seconds
	FTimerHandle DestroyTimerHandle;
	const float DestroyDelayTime = 5.0f;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ARogueProjectileBlackhole::DestroyTimerElapsed, DestroyDelayTime);
	
}

void ARogueProjectileBlackhole::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetRootComponent()->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
	
}

void ARogueProjectileBlackhole::DestroyTimerElapsed()
{
	ActiveBlackholeSoundComp->Deactivate();
	Destroy();
}


