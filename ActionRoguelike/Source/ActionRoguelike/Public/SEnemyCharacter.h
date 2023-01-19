// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SEnemyCharacter.generated.h"

class UCharacterMovementComponent;
class UCapsuleComponent;
class ASObstacle;
class UArrowComponent;

UCLASS()
class ACTIONROGUELIKE_API ASEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, Category = "Obstacle")
	TSubclassOf<AActor> Obstacle;
	//AActor* Solid;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FVector Arrive(const FVector& Target) const;
	FVector Pursuit(const AActor* TargetActor) const;
	FVector Seek(const FVector& Target) const;
	
	FVector Pursuit(const AActor*);
	
	FVector Wander(float DeltaTime);
	static float RandomClamped();
	FVector ObstacleAvoidance() const;
	FVector Flee(const FVector& Target, float TriggerDistance  = -1.0f) const;
	FVector Evade(const AActor* TargetActor, float TriggerDistance  = -1.0f) const;
	FVector AvoidanceVector(const FHitResult& Hit,const FVector& EndLocation) const;
	ASObstacle*  GetClosestObstacle();

	FVector GetClosestObstaclePosition();
	
	bool IsHiddenBy(const AActor* Actor) const;
	void WanderTimerFunction();
	void WanderRotate(const FVector& Pos);
	void FacePlayer();
	FTimerHandle MemberTimerHandle;
	

private:
	

	class UCharacterMovementComponent*	mpMovementComponent;
	class UCapsuleComponent*			mpCapsuleComponent;
	class UArrowComponent*				RightArrowComp;
	class UArrowComponent*				LeftArrowComp; 

	FVector								TargetPosition;
	FVector								mWanderTarget;
	AActor*								mpFollowActor;
	FVector								mFollowActorPrevLocation;
	TArray<FVector>						mCurrentPath;

	UCharacterMovementComponent*		mpFollowActorMovementComponent;

	mutable float						mLastPathFind;

	float DecelerationCoefficient = 5.0f;
	float LookAheadTimeModifier = 0.25f;

	float WanderJitter = 100.0f;

	
	float WanderRadius = 200.0f;

	
	float WanderDistance = 500.0f;

	
	float WanderMaxSpeed = 150.0f;

};
