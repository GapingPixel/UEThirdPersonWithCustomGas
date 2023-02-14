// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
	//LidMesh->SetRelativeRotation(FRotator(130,0,0));
	TargetPitch = 110;
	/*IsRotating = false;
	RotationVelocity = FRotator(50.0,0.0,0.0);*/

	SetReplicates(true);
	
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{

	//FVector::Equals
	//LidMesh->SetRelativeRotation(FRotator(0,0,0));
	/*float lidMeshRot = LidMesh->GetRelativeRotation().Pitch;
	if (!Used)
	{
		IsRotating = true;
		Used = true;
	}*/

	
	/*
	if ( FMath::IsNearlyEqual(lidMeshRot,180-TargetPitch,0.1f) )
	{
		LidMesh->SetRelativeRotation(FRotator(0,0,0));
	} else if ( FMath::IsNearlyEqual(lidMeshRot,0,0.1f) )
	{
		//LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
		IsRotating = true;
	}*/

	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
}


void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}

