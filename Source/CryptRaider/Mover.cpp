// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
	TargetLocation = OriginalLocation + MoveOffset;
	Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldMove && !m_Reached)
		Move(DeltaTime);
}

void UMover::Move(float deltaTime)
{
	FVector currentLocation = GetOwner()->GetActorLocation();

	FVector newLocation = FMath::VInterpConstantTo(currentLocation, TargetLocation, deltaTime, Speed);

	m_Reached = newLocation == TargetLocation;

	GetOwner()->SetActorLocation(newLocation);
}

void UMover::Invert()
{
	if (!IsInvertible) return;

	FVector temp = TargetLocation;
	TargetLocation = OriginalLocation;
	OriginalLocation = temp;
	m_Reached = false;
}