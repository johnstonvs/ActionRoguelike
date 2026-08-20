// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

#include "RogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Attribute Set
URogueActionSystemComponent* URogueAttributeSet::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}

// Health Attribute Set
URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	Health = FRogueAttribute(100.f);
	HealthMax = FRogueAttribute(Health.GetValue());
}

void URogueHealthAttributeSet::PostAttributeChanged()
{
	Health.Base = FMath::Clamp(Health.Base, 0.f, HealthMax.GetValue());
}

// Pawn Attribute Set
URoguePawnAttributeSet::URoguePawnAttributeSet()
{
	MoveSpeed = FRogueAttribute(550);
}

void URoguePawnAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::InitializeAttributes()
{
	Super::InitializeAttributes();
	
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::ApplyMoveSpeed()
{
	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue();
}

// Monster Attribute Set
URogueMonsterAttributeSet::URogueMonsterAttributeSet()
{
	MoveSpeed = FRogueAttribute(450);
}
