// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		bool ShouldMove = false;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	inline bool Reached() const { return m_Reached; }
	void Invert();

private:
	UPROPERTY(VisibleAnywhere)
		FVector OriginalLocation;
	UPROPERTY(VisibleAnywhere)
		FVector TargetLocation;

	UPROPERTY(EditAnywhere)
		FVector MoveOffset;

	UPROPERTY(EditAnywhere)
		float MoveTime = 4;
	UPROPERTY(VisibleAnywhere)
		float Speed;

	UPROPERTY(EditAnywhere)
		bool IsInvertible = false;

	UPROPERTY(VisibleAnywhere)
		bool m_Reached = false;

private:
	void Move(float deltaTime);
};
