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
	if (!AttributesCost.IsEmpty())
	{
		for (TPair<FGameplayTag, float> Cost : AttributesCost)
		{
			OwningComp->ApplyAttributeChange(Cost.Key, -(Cost.Value), Base);
		}
	}
	
	OwningComp->ActiveGameplayTags.AppendTags(GrantTags);
}

void URogueAction::StopAction_Implementation()
{
	bIsRunning = false;
	
	float GameTime = GetWorld()->TimeSeconds;
	UE_LOGFMT(LogTemp, Log, "Stopped Action {ActionName} - {WorldTime}",
		("ActionName", ActionName.ToString()),
		("WorldTime", GameTime));
	
	CooldownUntil = GetWorld()->TimeSeconds + CooldownTime;
	
	OwningComp->ActiveGameplayTags.RemoveTags(GrantTags);
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
	
	if (OwningComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	// If attribute cost value is lower than required, return false
	if (!AttributesCost.IsEmpty())
	{
		for (TPair<FGameplayTag, float> Cost : AttributesCost)
		{
			if (OwningComp->GetAttributeValue(Cost.Key) < Cost.Value)
			{
				// Not enough resources
				UE_LOGFMT(LogTemp, Log, "Not enough {AttributeName} to activate {ActionName}. "
										"Have {Available} and need {Required}.",
										("AttributeName", Cost.Key.ToString()),
										("ActionName", ActionName.ToString()),
										("Available", OwningComp->GetAttributeValue(Cost.Key)),
										("Required", Cost.Value));
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
