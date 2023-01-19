// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SEnemy.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector Pursuit(const AActor*);
	FVector Wander(float DeltaTime);

private:
	static float RandomClamped();

	bool IsHiddenBy(const AActor* Actor) const;

	class UCharacterMovementComponent*	mpMovementComponent;
	class UCapsuleComponent*			mpCapsuleComponent;

	FVector								mWanderTarget;
	AActor*								mpFollowActor;
	FVector								mFollowActorPrevLocation;
	TArray<FVector>						mCurrentPath;

	UCharacterMovementComponent*		mpFollowActorMovementComponent;

	mutable float						mLastPathFind;

	float DecelerationCoefficient = 1.0f;

};
