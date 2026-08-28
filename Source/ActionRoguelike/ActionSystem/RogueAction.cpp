// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "RogueActionSystemComponent.h"
#include "Engine/World.h"
#include "Logging/StructuredLog.h"


void URogueAction::StartAction_Implementation()
{
	bIsRunning = true;
	
	float GameTime = GetWorld()->TimeSeconds;
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName} - {WorldTime}",
		("ActionName", ActionName.ToString()),
		("WorldTime", GameTime));
	
	// If action has attributes to be consumed, consume attributes
	if (!AttributesToConsume.IsEmpty())
	{
		for (TTuple<FGameplayTag, float> AttributeToBeConsumed : AttributesToConsume)
		{
			GetOwningComponent()->ApplyAttributeChange(AttributeToBeConsumed.Key, -(AttributeToBeConsumed.Value), Base);
		}
	}
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantTags);
}

void URogueAction::StopAction_Implementation()
{
	bIsRunning = false;
	
	float GameTime = GetWorld()->TimeSeconds;
	UE_LOGFMT(LogTemp, Log, "Stopped Action {ActionName} - {WorldTime}",
		("ActionName", ActionName.ToString()),
		("WorldTime", GameTime));
	
	CooldownUntil = GetWorld()->TimeSeconds + CooldownTime;
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantTags);
}

bool URogueAction::CanStart() const
{
	if (IsRunning())
	{
		return false;
	}
	
	if (GetCooldownTimeRemaining() > 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("Cooldown remaining: %f"), GetCooldownTimeRemaining());
		return false;
	}
	
	if (GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	// If attribute to consume amount is lower than required, return false
	if (!AttributesToConsume.IsEmpty())
	{
		for (TTuple<FGameplayTag, float> AttributeToBeConsumed : AttributesToConsume)
		{
			if (GetOwningComponent()->GetAttributeValue(AttributeToBeConsumed.Key) < AttributeToBeConsumed.Value)
			{
				return false;
			}
		}
	}
	
	return true;
}

float URogueAction::GetCooldownTimeRemaining() const
{
	return FMath::Max(0.f, CooldownUntil - GetWorld()->TimeSeconds);
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
		return Cast<URogueActionSystemComponent>(GetOuter());
}
