// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	static const FName GrabbedTag;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void Grab();

	UFUNCTION(BlueprintCallable)
		void Release();

private:
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere)
		float MaxGrabDistance = 200.0f;

	UPROPERTY(EditAnywhere)
		float GrabRadius = 10.0f;

	UPROPERTY(EditAnywhere)
		float HoldDistance = 100.0f;

private:
	bool GetGrabbableInReach(FHitResult& hit) const;
	void StartHolding(const FHitResult& hit);
	void UpdateGrabbedObject();
	void AddTag(AActor* actor);
	void RemoveTag(AActor* actor);
};
