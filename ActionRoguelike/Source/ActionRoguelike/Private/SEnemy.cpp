// Fill out your copyright notice in the Description page of Project Settings.


#include "SEnemy.h"

// Sets default values
ASEnemy::ASEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



/*FVector ASEnemy::Pursuit(const AActor* TargetActor)
{
	AActor* Owner = GetOwner();
	FVector TargetActorLocation = TargetActor->GetActorLocation();
	FVector TargetActorVelocity = TargetActor->GetVelocity();

	FVector ToTargetActor = TargetActorLocation - Owner->GetActorLocation();
      
	float LookAheadTime = (ToTargetActor.Size() / mpMovementComponent->GetMaxSpeed() + TargetActorVelocity.Size()) / LookAheadTimeModifier;
	return Seek(TargetActorLocation + TargetActorVelocity * LookAheadTime);
}*/
/*
FVector ASEnemy::Wander(float DeltaTime)
{
	float Jitter = WanderJitter * DeltaTime;
	mWanderTarget += FVector(RandomClamped() * Jitter, RandomClamped() * Jitter, 0);
	mWanderTarget.Normalize();
	mWanderTarget *= WanderRadius;
	FVector Target = mWanderTarget + FVector(WanderDistance, 0, 0);
	Target = GetOwner()->GetTransform().TransformPosition(Target);
	FVector ToPos = Target - GetOwner()->GetActorLocation();
	ToPos.Normalize();
	FVector DesiredVelocity = ToPos * WanderMaxSpeed;
	return DesiredVelocity - GetOwner()->GetVelocity();
}*/
