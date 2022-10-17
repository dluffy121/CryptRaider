// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Grabber.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* actor = GetAcceptableActor();
	if (actor)
	{
		if (actor->IsAttachedTo(GetOwner())) return;
		UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
		if (component)
			component->SetSimulatePhysics(false);
		actor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		actor->SetActorRelativeLocation(KeyAttachLocation);
		actor->SetActorRelativeRotation(KeyAttachRotation);
		if (component)
			component->SetSimulatePhysics(true);
		if (Mover)
			Mover->ShouldMove = true;
	}
	else
	{
		if (Mover)
		{
			Mover->ShouldMove = false;
			if (Mover->Reached())
				Mover->Invert();
		}
	}
}

void UTriggerComponent::SetMover(UMover* mover)
{
	Mover = mover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> actors;
	GetOverlappingActors(actors);

	for (AActor* actor : actors)
	{
		if (actor->ActorHasTag(AcceptedTriggerTag)
			&& !actor->ActorHasTag(UGrabber::GrabbedTag))
			return actor;
	}

	return nullptr;
}
