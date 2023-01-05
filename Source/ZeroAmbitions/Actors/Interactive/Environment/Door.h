// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactive/Interface/Interactive.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class ZEROAMBITIONS_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ADoor();

	virtual void Interact(AZABaseCharacter* Character) override;

	virtual FName GetActionEventName() const override;
	
	virtual void Tick(float DeltaTime) override;

	virtual bool HasOnInteractionCallback() const override;
	virtual FDelegateHandle AddOnInteractionUFunction(UObject* Object, const FName& FunctionName) override;
	virtual void RemoveOnInteractionDelegate(FDelegateHandle DelegateHandle) override;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive | Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive | Door")
	USceneComponent* DoorPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive | Door")
	float AngleClosed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive | Door")
	float AngleOpened = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive | Door")
	UCurveFloat* DoorAnimationCurve;

	IInteractable::FOnInteraction OnInteractionEvent;
	
	virtual void BeginPlay() override;

private:

	void InteractWithDoor();
	
	UFUNCTION()
	void UpdateDoorAnimation(float Alpha);

	UFUNCTION()
	void OnDoorAnimationFinished();
	
	FTimeline DoorOpenAnimTimeline;

	bool bIsOpened = false;

};
