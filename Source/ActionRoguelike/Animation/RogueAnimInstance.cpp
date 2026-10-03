// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAnimInstance.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/Actor.h"

void URogueAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	ActionComp = GetOwningActor()->FindComponentByClass<URogueActionSystemComponent>();
}

void URogueAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (ActionComp)
	{
		// @TODO: Update to use delegates from action system.
		bIsSprinting = ActionComp->ActiveGameplayTags.HasTag(SharedGameplayTags::StatusEffect_Sprinting);
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(SharedGameplayTags::StatusEffect_Stunned);
	}
}
