// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Spawners/AICharacterSpawner.h"

#include "Actors/Interactive/Interface/Interactive.h"
#include "AI/Characters/ZAAIBaseCharacter.h"

AAICharacterSpawner::AAICharacterSpawner()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnerRoot"));
	SetRootComponent(SceneRoot);
}

void AAICharacterSpawner::SpawnAI()
{
	if(!bCanSpawn || !IsValid(CharacterClass))
	{
		return;
	}

	AZAAIBaseCharacter* AICharacter = GetWorld()->SpawnActor<AZAAIBaseCharacter>(CharacterClass, GetTransform());
	if(!IsValid(AICharacter))
	{
		return;
	}
	if(!IsValid(AICharacter->Controller))
	{
		AICharacter->SpawnDefaultController();
	}

	if(bDoOnce)
	{
		UnSubscribeFromTrigger();
		bCanSpawn = false;
	}
}

void AAICharacterSpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_STRING_CHECKED(AAICharacterSpawner, SpawnTrigger))
	{
		SpawnTrigger = SpawnTriggerActor;
		if(!SpawnTrigger.GetInterface())
		{
			if(!SpawnTrigger->HasOnInteractionCallback())
			{
				SpawnTriggerActor = nullptr;
				SpawnTrigger = nullptr;
			}
		}
		else
		{
			SpawnTriggerActor = nullptr;
			SpawnTrigger = nullptr;
		}
	}
}

void AAICharacterSpawner::BeginPlay()
{
	Super::BeginPlay();

	if(SpawnTrigger.GetInterface())
	{
		SpawnTrigger->AddOnInteractionUFunction(this, FName("SpawnAI"));
	}
	
	if(bIsSpawnOnStart)
	{
		SpawnAI();
	}
}

void AAICharacterSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnSubscribeFromTrigger();
	Super::EndPlay(EndPlayReason);
}

void AAICharacterSpawner::UnSubscribeFromTrigger()
{
	if(TriggerHandle.IsValid() && SpawnTrigger.GetInterface())
	{
		SpawnTrigger->RemoveOnInteractionDelegate(TriggerHandle);
	}
}




