// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueGameTypes.h"
#include "TimerManager.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectile.h"

TAutoConsoleVariable<float> CVarProjectileAdjustmentsDebugDrawing(TEXT("game.projectile.DebugDraw"), false, TEXT("Enable projectile aim adjustment debug rendering. (0 = off, > 0 is duration>"), ECVF_Cheat);

ARoguePlayerCharacter::ARoguePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp"));
	
	MuzzleSocketName = "Muzzle_01";
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ThisClass::Jump);
	
	// Projectile Attacks
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ThisClass::StartProjectileAttack, MagicProjectileClass);
	EnhancedInput->BindAction(Input_BlackholeAttack, ETriggerEvent::Triggered, this, &ThisClass::StartProjectileAttack, BlackholeProjectileClass);
	EnhancedInput->BindAction(Input_Blink, ETriggerEvent::Triggered, this, &ThisClass::StartProjectileAttack, BlinkProjectileClass);
	
}

void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	
	// Forward/Back
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	// Right/Left
	FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::StartProjectileAttack(TSubclassOf<ARogueProjectile> ProjectileClass)
{
	PlayAnimMontage(AttackMontage);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	UGameplayStatics::PlaySound2D(this, CastingSound);
	
	FTimerHandle AttackTimerHandle;
	const float AttackDelayTime = 0.2f;
	
	// Set delegate to pass in projectile type to be used
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ThisClass::AttackTimerElapsed, ProjectileClass);
	
	GetWorldTimerManager().SetTimer(AttackTimerHandle, TimerDelegate, AttackDelayTime, false);
}

void ARoguePlayerCharacter::AttackTimerElapsed(TSubclassOf<ARogueProjectile> ProjectileClass)
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// Improve projectile direction for better aiming
	FVector EyeLocation = CameraComponent->GetComponentLocation();
	FRotator EyeRotation = GetControlRotation();
	
	// End location is where player is looking up to 5000 units
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 5000.f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	UWorld* World = GetWorld();
	
	FVector AdjustedTargetLocation;
	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
	{
		// Target location is the hit location
		AdjustedTargetLocation = Hit.Location;
	}
	else
	{
		// Target location is the trace end location
		AdjustedTargetLocation = TraceEnd;
	}
	
	// Get rotation from SpawnLocation to AdjustedTargetLocation
	FRotator SpawnRotation = (AdjustedTargetLocation - SpawnLocation).Rotation();
	
	AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	MoveIgnoreActorAdd(NewProjectile);
	
#if !UE_BUILD_SHIPPING // Aim adjustment debug drawings
	float DebugDrawDuration = CVarProjectileAdjustmentsDebugDrawing.GetValueOnGameThread();
	if (DebugDrawDuration > 0.f)
	{
		// Hit location or trace end
		DrawDebugBox(World, AdjustedTargetLocation, FVector(20.f), FColor::Green, false, DebugDrawDuration);
	
		// Adjustment line trace
		DrawDebugLine(World, EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);
	
		// New Projectile path
		DrawDebugLine(World, SpawnLocation, AdjustedTargetLocation, FColor::Yellow, false, DebugDrawDuration);
	
		// Original projectile path
		DrawDebugLine(World, SpawnLocation, SpawnLocation + (GetControlRotation().Vector() * 5000.f), FColor::Purple, false, DebugDrawDuration);
	}
#endif
	
}

void ARoguePlayerCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	// Died?
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(nullptr);
		
		GetMovementComponent()->StopMovementImmediately();
		
		PlayAnimMontage(DeathMontage);
	}
}

float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                        class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ActionSystemComponent->ApplyHealthChange(-ActualDamage);
	
	return ActualDamage;
}
