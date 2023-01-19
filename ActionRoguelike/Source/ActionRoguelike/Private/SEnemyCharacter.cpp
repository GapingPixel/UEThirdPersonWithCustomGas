// Fill out your copyright notice in the Description page of Project Settings.


#include "SEnemyCharacter.h"

#include "SObstacle.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Sets default values
ASEnemyCharacter::ASEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

void ASEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	mpMovementComponent	= FindComponentByClass<UCharacterMovementComponent>();
	mpCapsuleComponent	= FindComponentByClass<UCapsuleComponent>();
	RightArrowComp = Cast<UArrowComponent>(GetDefaultSubobjectByName(TEXT("RightArrow")));
	LeftArrowComp = Cast<UArrowComponent>(GetDefaultSubobjectByName(TEXT("LeftArrow")));
}


// Called when the game starts or when spawned
void ASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ASEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TargetPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();
	//FVector Velocity = Evade(GetWorld()->GetFirstPlayerController()->GetPawn());//+ Arrive(TargetPosition);//Pursuit(GetWorld()->GetFirstPlayerController()->GetPawn()) + ObstacleAvoidance();
	//FVector Velocity = Evade(GetClosestObstacle()); //+ Arrive(TargetPosition)
	
	if (GetDistanceTo(GetWorld()->GetFirstPlayerController()->GetPawn()) < 1500)//If closer than 1500 Units Arrive to Player's Pos, else Wander Around
	{
		if (ObstacleAvoidance().Size()== 0)
		{
			FVector Velocity =  Arrive(TargetPosition);
			FacePlayer();
			CurrentLocation = CurrentLocation + (Velocity*DeltaTime);
			GEngine->AddOnScreenDebugMessage(2,0.0f,FColor::Red,"Running Arrive");
		} else
		{
			FVector Velocity =  ObstacleAvoidance()+ Flee(GetClosestObstaclePosition());//Wander(DeltaTime);
			CurrentLocation = CurrentLocation + (Velocity*DeltaTime);
			GEngine->AddOnScreenDebugMessage(2,0.0f,FColor::Red,"Flee & Avoidance");
		}
	} else
	{
		FVector Velocity =  Wander(DeltaTime);
		CurrentLocation = CurrentLocation + (Velocity*DeltaTime);
		
		GEngine->AddOnScreenDebugMessage(2,0.0f,FColor::Red,"Running Wander");
	}

	SetActorLocation(CurrentLocation);

	/*bool bHasTag = this->ActorHasTag(FName(TEXT("Enemy")));
	if (bHasTag) {
		GEngine->AddOnScreenDebugMessage(2,0.0f,FColor::Red,"TAG IT WORRKS");
	}*/
	
}

// Called to bind functionality to input
void ASEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector ASEnemyCharacter::Arrive(const FVector& Target) const
{
	//const FVector& Target = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector ToTarget = Target - GetActorLocation();
	float Distance = ToTarget.Size();

	if (Distance > 100)
	{
		float Speed = Distance / DecelerationCoefficient;
		Speed = FMath::Min(Speed, mpMovementComponent->GetMaxSpeed());
		
		FVector DesiredVelocity = ToTarget / Distance * Speed;

		return DesiredVelocity - GetVelocity();
	}

	return FVector(0, 0, 0);
}

FVector ASEnemyCharacter::Pursuit(const AActor* TargetActor) const
{
	FVector TargetActorLocation = TargetActor->GetActorLocation();
	FVector TargetActorVelocity = TargetActor->GetVelocity();

	FVector ToTargetActor = TargetActorLocation - GetActorLocation();
      
	float LookAheadTime = (ToTargetActor.Size() / mpMovementComponent->GetMaxSpeed() + TargetActorVelocity.Size()) / LookAheadTimeModifier;
	return Seek(TargetActorLocation + TargetActorVelocity * LookAheadTime);
}

FVector ASEnemyCharacter::Seek(const FVector& Target) const
{
	
	FVector ToTarget = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
	ToTarget.Normalize();
	FVector DesiredVelocity = ToTarget * mpMovementComponent->GetMaxSpeed();

	return DesiredVelocity - GetVelocity();
}

void ASEnemyCharacter::FacePlayer()
{
	FVector Target = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector direction = Target-GetActorLocation();
	direction.Normalize();

	auto lookat = FRotationMatrix::MakeFromX(direction).Rotator();
	lookat.Pitch = 0.0f;
	lookat.Roll = 0.0f;

	SetActorRotation(lookat);
}

FVector ASEnemyCharacter::Wander(float DeltaTime)
{
	float Jitter = WanderJitter * DeltaTime;
	mWanderTarget += FVector(RandomClamped()* Jitter, RandomClamped() * Jitter, 0);
	mWanderTarget.Normalize();
	mWanderTarget *= WanderRadius;
	FVector Target = mWanderTarget + FVector(WanderDistance, 0, 0);
	Target = GetTransform().TransformPosition(Target);
	FVector ToPos = Target - GetActorLocation();
	ToPos.Normalize();
	FVector DesiredVelocity = ToPos * WanderMaxSpeed;

	//FTimerDelegate RespawnDelegate = FTimerDelegate::WanderRotate(ToPos );
	/*FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ASEnemyCharacter::WanderRotate, ToPos);*/
	
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ASEnemyCharacter::WanderTimerFunction, 1.0f, true, 1.0);

	return DesiredVelocity - GetVelocity();
}

void ASEnemyCharacter::WanderTimerFunction()
{
	FVector Target = mWanderTarget + FVector(WanderDistance, 0, 0);
	Target = GetTransform().TransformPosition(Target);
	FVector ToPos = Target - GetActorLocation();
	ToPos.Normalize();
	WanderRotate(ToPos);
}

void ASEnemyCharacter::WanderRotate(const FVector& Pos) 
{
	auto lookat = FRotationMatrix::MakeFromX(Pos).Rotator();
	lookat.Pitch = 0.0f;
	lookat.Roll = 0.0f;
	SetActorRotation(lookat);
	
}


float ASEnemyCharacter::RandomClamped()
{
	return FMath::FRand() - FMath::FRand();
}

FVector ASEnemyCharacter::ObstacleAvoidance() const
{
	//AActor* pOwner = GetOwner();
	FHitResult Hit;
	float CollisionLookahead			= 100.0f;
	const FVector StartLocation			= mpCapsuleComponent->GetComponentLocation();
	const FVector EndLocation			= StartLocation + GetActorForwardVector() * CollisionLookahead;

	const FVector StartLocationRight			= RightArrowComp->GetComponentLocation();
	const FVector EndLocationRight			= StartLocationRight + GetActorForwardVector() * CollisionLookahead;

	const FVector StartLocationLeft			= LeftArrowComp->GetComponentLocation();
	const FVector EndLocationLeft			= StartLocationLeft + GetActorForwardVector() * CollisionLookahead;
	
	const ECollisionChannel Channel		= mpCapsuleComponent->GetCollisionObjectType();

	FCollisionQueryParams QueryParams(NAME_None, false, this);
	FCollisionResponseParams ResponseParam;
	mpCapsuleComponent->InitSweepCollisionParams(QueryParams, ResponseParam);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, Channel, QueryParams, ResponseParam);
	
	if (bHit)
	{
		return  AvoidanceVector(Hit, EndLocation);
	}
	bool bHitRight = GetWorld()->LineTraceSingleByChannel(Hit, StartLocationRight, EndLocationRight, Channel, QueryParams, ResponseParam);

	if (bHitRight)
	{
		return  AvoidanceVector(Hit, EndLocationRight);
	}
	bool bHitLeft = GetWorld()->LineTraceSingleByChannel(Hit, StartLocationLeft, EndLocationLeft, Channel, QueryParams, ResponseParam);

	if (bHitLeft)
	{
		return  AvoidanceVector(Hit, EndLocationLeft);
	}
	return FVector(0, 0, 0);
}

FVector ASEnemyCharacter::AvoidanceVector(const FHitResult& Hit,const FVector& EndLocation) const
{
	if (Hit.GetActor()->ActorHasTag(FName(TEXT("Obstacle")))) {
		FVector PenetratedAlongHit = Hit.ImpactPoint - EndLocation;
		FVector PenetratedAlongNormal = PenetratedAlongHit.ProjectOnToNormal(Hit.ImpactNormal);
			
		// Hit.PenetrationDepth will always be 0 because it's a SingleLineTrace, not Multi
		float PenetrationDepth = PenetratedAlongNormal.Size();

		//FString DebugMSG = FString::Printf(TEXT("Rotate Value: %s"),*FString::SanitizeBool(bHit));
		GEngine->AddOnScreenDebugMessage(1,0.0f,FColor::Green,"FoundObstacle");

		FVector check = Hit.ImpactNormal * PenetrationDepth;
		FString DebugMSG = check.ToString();//FString::Printf(TEXT("Avoid Vector: %s"),*FString::SanitizeBool(Hit.ImpactNormal * PenetrationDepth));
		GEngine->AddOnScreenDebugMessage(2,0.0f,FColor::Red,DebugMSG);
		return (Hit.ImpactNormal * PenetrationDepth);
	} else
	{
		return FVector(0, 0, 0);
	}
}

FVector ASEnemyCharacter::Flee(const FVector& Target, float TriggerDistance) const
{
	
	FVector FromTarget = GetActorLocation() - Target;

	if (TriggerDistance < 0 || FromTarget.SizeSquared() <= TriggerDistance * TriggerDistance)
	{
		FromTarget.Normalize();
		FVector DesiredVelocity = FromTarget * mpMovementComponent->GetMaxSpeed();
		return DesiredVelocity - GetVelocity();
	}

	return FVector(0, 0, 0);
}

FVector ASEnemyCharacter::Evade(const AActor* TargetActor, float TriggerDistance) const
{
	//AActor* Owner = GetOwner();
	FVector TargetActorLocation = TargetActor->GetActorLocation();
	FVector TargetActorVelocity = TargetActor->GetVelocity();

	FVector ToTargetActor = TargetActorLocation - GetActorLocation();

	if (TriggerDistance < 0 || ToTargetActor.SizeSquared() <= TriggerDistance * TriggerDistance)
	{
		float LookAheadTime = (ToTargetActor.Size() / mpMovementComponent->GetMaxSpeed() + TargetActorVelocity.Size()) / LookAheadTimeModifier;
		return Flee( TargetActorLocation + TargetActorVelocity * LookAheadTime);
	}

	return FVector(0, 0, 0);
}



ASObstacle* ASEnemyCharacter::GetClosestObstacle()
{
	ASObstacle* ClosestObstacle = nullptr;
	for (TObjectIterator<ASObstacle> Itr; Itr; ++Itr) //for all Obstacles in the world
		{
			if (ClosestObstacle == nullptr) { ClosestObstacle = *Itr; continue; }
			if (Itr->GetDistanceTo(this) < ClosestObstacle->GetDistanceTo(this))
			{
				ClosestObstacle = *Itr;
			}
		}
	return ClosestObstacle;
}

FVector ASEnemyCharacter::GetClosestObstaclePosition()
{
	return GetClosestObstacle()->GetActorLocation();

}

