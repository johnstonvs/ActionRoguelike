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
	// Set profile to go through objects and overlap
	SphereComponent->SetCollisionProfileName("ProjectileOverlap");
	SphereComponent->SetSphereRadius(20.f);
	
	
	// Slow
	ProjectileMovementComponent->InitialSpeed = 500.f;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(SphereComponent);
	RadialForceComponent->Radius = 1200.f;
	RadialForceComponent->ForceStrength = -700000.f;
	// Avoid sucking players into the blackhole by removing pawn actors from the collision channel
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
	// Despawn after 5 seconds
	InitialLifeSpan = 5.f;
}

void ARogueProjectileBlackhole::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlappedActor);
	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(BlackholeEffect, SphereComponent, NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	ActiveBlackholeSoundComp = UGameplayStatics::SpawnSoundAttached(BlackholeSound, SphereComponent);
	
	// Deactivate sounds before getting destroyed
	OnDestroyed.AddDynamic(this, &ThisClass::HandleDestroyed);
}

void ARogueProjectileBlackhole::OnSphereOverlappedActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetRootComponent()->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}

void ARogueProjectileBlackhole::HandleDestroyed(AActor* DestroyedActor)
{
	if (ActiveBlackholeSoundComp)
	{
		ActiveBlackholeSoundComp->Deactivate();
	}
}

