// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	/*APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		//FVector pos = FVector(125.0f, 530.0f, 88.0f);
		GetBlackboardComponent()->SetValueAsVector(FName("MoveToLocation"), MyPawn->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), MyPawn);
		
	}*/
}
