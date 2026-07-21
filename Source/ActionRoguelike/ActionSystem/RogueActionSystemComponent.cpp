// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{

}

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
	float OldHealth = Attributes.Health;
	
	Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.f, Attributes.MaxHealth);

	if (!FMath::IsNearlyEqual(OldHealth, Attributes.Health))
	{
		OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
	}

	UE_LOG(LogTemp, Log, TEXT("New Health: %f, MaxHealth: %f"), Attributes.Health, Attributes.MaxHealth);
}

bool URogueActionSystemComponent::IsFullHealth()
{
	return FMath::IsNearlyEqual(Attributes.Health, Attributes.MaxHealth);
}
