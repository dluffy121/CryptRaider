// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorScaler.h"
#include "Grabber.h"

// Sets default values for this component's properties
UActorScaler::UActorScaler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UActorScaler::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UActorScaler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ScalingActor)
	{
		ScaleActor(DeltaTime);
		if (!IsScaling)
		{
			UPrimitiveComponent* component = Cast<UPrimitiveComponent>(ScalingActor->GetRootComponent());
			if (component)
				component->SetSimulatePhysics(true);
			ScalingActor = nullptr;
		}

		return;
	}

	AActor* actor = GetAcceptableActor();
	if (actor)
	{
		ScalingActor = actor;

		UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
		if (component)
			component->SetSimulatePhysics(false);
		actor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		actor->SetActorRelativeLocation(ActorAttachLocation);

		Speed = FVector::Distance(FinalScale, actor->GetActorScale3D()) / ScaleTime;

		ScaleActor(DeltaTime);
	}
}

AActor* UActorScaler::GetAcceptableActor() const
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

void UActorScaler::ScaleActor(float deltaTime)
{
	FVector currentScale = ScalingActor->GetActorScale3D();

	FVector newScale = FMath::VInterpTo(currentScale, FinalScale, deltaTime, Speed);

	IsScaling = newScale != FinalScale;

	ScalingActor->SetActorScale3D(newScale);
}