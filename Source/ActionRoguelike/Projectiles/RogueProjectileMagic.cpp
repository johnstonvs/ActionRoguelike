// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "ActionSystem/RogueActionEffect.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileMagic::ARogueProjectileMagic()
{
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	
	InitialLifeSpan = 8.0f;
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Keep base implementation, damage applied before projectile is destroyed
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	
	FVector HitFromDirection = GetActorRotation().Vector();
	
	UGameplayStatics::ApplyPointDamage(OtherActor, 10.f, HitFromDirection, Hit, GetInstigatorController(), this, DmgTypeClass);
	
	if (EffectOnHit)
	{
		URogueActionSystemComponent* ActionComp = OtherActor->FindComponentByClass<URogueActionSystemComponent>();
		if (ActionComp) // Not everything has one
		{
			ActionComp->GrantAction(EffectOnHit);
		}
	}
}
