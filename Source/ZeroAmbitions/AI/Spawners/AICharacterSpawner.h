// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AICharacterSpawner.generated.h"

class IInteractable;
class AZAAIBaseCharacter;
UCLASS()
class ZEROAMBITIONS_API AAICharacterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AAICharacterSpawner();

	UFUNCTION()
	void SpawnAI();

protected:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Spawner")
	TSubclassOf<AZAAIBaseCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Spawner")
	bool bIsSpawnOnStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Spawner")
	bool bDoOnce = false;

	// An actor implementing IInteractable interface
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Spawner")
	AActor* SpawnTriggerActor;

private:
	bool bCanSpawn = true;

	void UnSubscribeFromTrigger();

	UPROPERTY()
	TScriptInterface<IInteractable> SpawnTrigger;

	FDelegateHandle TriggerHandle;

};
