// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapDoor.h"
#include "Grabber.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UTrapDoor::UTrapDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTrapDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorCloseAngle = GetOwner()->GetActorRotation();
	DoorOpenAngle = GetOwner()->GetActorRotation().Add(
		DoorOpeningDirection.Pitch * DoorOpeningAngle,
		DoorOpeningDirection.Yaw * DoorOpeningAngle,
		DoorOpeningDirection.Roll * DoorOpeningAngle);
	OpenCloseSpeed =  FMath::Abs(DoorOpeningAngle) / OpenCloseDuration;
}

void UTrapDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* actor = GetAcceptableActor();
	if (actor)
	{
		OpenDoor(DeltaTime);

		if (actor->IsAttachedTo(GetOwner())) return;
		UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
		if (component)
			component->SetSimulatePhysics(false);
		actor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		actor->SetActorLocation(KeyAttachLocation);

		if (component)
			component->SetSimulatePhysics(true);
	}
	else
	{
		CloseDoor(DeltaTime);
	}

	IsOpen = GetOwner()->GetActorRotation() != DoorCloseAngle;
}

AActor* UTrapDoor::GetAcceptableActor() const
{
	TArray<AActor*> actors;
	GetOverlappingActors(actors);

	for (AActor* actor : actors)
	{
		if (actor->ActorHasTag(AcceptedKeyTag)
			&& !actor->ActorHasTag(UGrabber::GrabbedTag))
			return actor;
	}

	return nullptr;
}

void UTrapDoor::OpenDoor(float deltaTime)
{
	FRotator currentRotation = GetOwner()->GetActorRotation();

	FRotator newRotation = FMath::RInterpTo(currentRotation, DoorOpenAngle, deltaTime, OpenCloseDuration);

	GetOwner()->SetActorRotation(newRotation);
}

void UTrapDoor::CloseDoor(float deltaTime)
{
	FRotator currentRotation = GetOwner()->GetActorRotation();

	FRotator newRotation = FMath::RInterpTo(currentRotation, DoorCloseAngle, deltaTime, OpenCloseDuration);

	GetOwner()->SetActorRotation(newRotation);
}