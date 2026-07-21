// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FRogueAttributeSet
{
	GENERATED_BODY()
	
	FRogueAttributeSet() :
	MaxHealth(100.f),
	Health(100.f) {}
	
	UPROPERTY(BlueprintReadWrite)
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly)
	float Health;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, OldHealth);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	void ApplyHealthChange(float InValueChange);
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION()
	bool IsFullHealth();
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FRogueAttributeSet Attributes;
	
	
public:
	
	URogueActionSystemComponent();

};
