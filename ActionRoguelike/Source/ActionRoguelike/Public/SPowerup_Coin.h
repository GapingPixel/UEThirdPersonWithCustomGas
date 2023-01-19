// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Coin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Coin : public ASPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Coin")
	int32 CreditGain;

	//UStaticMeshComponent* MeshComp;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	ASPowerup_Coin();
	
};
