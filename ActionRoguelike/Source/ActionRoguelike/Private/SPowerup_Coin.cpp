// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Coin.h"
#include "SPlayerState.h"


ASPowerup_Coin::ASPowerup_Coin()
{
	/*MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);*/

	CreditGain = 80;
}

void ASPowerup_Coin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (ASPlayerState * PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditGain);
		HideAndCooldownPowerup();
	}	
}
