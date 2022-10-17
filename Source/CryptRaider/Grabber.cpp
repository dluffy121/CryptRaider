// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

const FName UGrabber::GrabbedTag = "Grabbed";

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGrabbedObject();
}

void UGrabber::Grab()
{
	if (!PhysicsHandle) return;

	FHitResult hit;
	if (GetGrabbableInReach(hit))
	{
		StartHolding(hit);
		AddTag(hit.GetActor());
		UE_LOG(LogTemp, Display, TEXT("Grabbed: %s"), *hit.GetComponent()->GetName());
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle || !PhysicsHandle->GetGrabbedComponent())
		return;

	UE_LOG(LogTemp, Display, TEXT("Released: %s"), *PhysicsHandle->GetGrabbedComponent()->GetName());
	RemoveTag(PhysicsHandle->GetGrabbedComponent()->GetOwner());
	PhysicsHandle->ReleaseComponent();
}

bool UGrabber::GetGrabbableInReach(FHitResult& hit) const
{
	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		hit,
		GetComponentLocation(),
		GetComponentLocation() + GetForwardVector() * MaxGrabDistance,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere);
}

void UGrabber::StartHolding(const FHitResult& hit)
{
	UPrimitiveComponent* hitComponent = hit.GetComponent();

	hitComponent->WakeAllRigidBodies();

	AActor* actor =  hit.GetActor();
	actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	PhysicsHandle->GrabComponentAtLocationWithRotation(
		hitComponent,
		NAME_None,
		hit.ImpactPoint,
		GetComponentRotation());
}

void UGrabber::UpdateGrabbedObject()
{
	if (!PhysicsHandle || !PhysicsHandle->GetGrabbedComponent())
		return;

	PhysicsHandle->SetTargetLocationAndRotation(
		GetComponentLocation() + GetForwardVector() * HoldDistance,
		GetComponentRotation());
}

void UGrabber::AddTag(AActor* actor)
{
	actor->Tags.Add(GrabbedTag);
}

void UGrabber::RemoveTag(AActor* actor)
{
	actor->Tags.Remove(GrabbedTag);
}