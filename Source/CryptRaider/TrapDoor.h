// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TrapDoor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTrapDoor : public UBoxComponent
{
	GENERATED_BODY()

public:
// Sets default values for this component's properties
	UTrapDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
		FName AcceptedKeyTag;

	UPROPERTY(EditAnywhere)
		FVector KeyAttachLocation;

	UPROPERTY(EditAnywhere)
		float DoorOpeningAngle;

	UPROPERTY(EditAnywhere)
		FRotator DoorOpeningDirection;

	UPROPERTY(EditAnywhere)
		float OpenCloseDuration;

	UPROPERTY(VisibleAnywhere)
		float OpenCloseSpeed;

	UPROPERTY(VisibleAnywhere)
		FRotator DoorOpenAngle;

	UPROPERTY(VisibleAnywhere)
		FRotator DoorCloseAngle;

	UPROPERTY(VisibleAnywhere)
		bool IsOpen = false;

private:
	AActor* GetAcceptableActor() const;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
};
