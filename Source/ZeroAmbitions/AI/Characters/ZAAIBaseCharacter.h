// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ZABaseCharacter.h"
#include "ZAAIBaseCharacter.generated.h"

/**
 * 
 */
class UAIPatrollingComponent;
class UBehaviorTree;
UCLASS(Blueprintable)
class ZEROAMBITIONS_API AZAAIBaseCharacter : public AZABaseCharacter
{
	GENERATED_BODY()

public:
	AZAAIBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetPatrollingComponent() const;

	UBehaviorTree* GetBehaviorTree() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPatrollingComponent* AIPatrollingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
};
