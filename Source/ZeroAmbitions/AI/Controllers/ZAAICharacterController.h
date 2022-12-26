// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controllers/ZAAIController.h"
#include "ZAAICharacterController.generated.h"

/**
 * 
 */

class AZAAIBaseCharacter;
UCLASS()
class ZEROAMBITIONS_API AZAAICharacterController : public AZAAIController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TargetReachedRadius = 100.0f;
private:

	void TryMoveToNextTarget();

	bool IsTargetReached(FVector TargetLocation) const;
	bool bIsPatrolling = false;

	TWeakObjectPtr<AZAAIBaseCharacter> CachedAICharacter;
};
