// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterInventoryComponent.generated.h"

class UInventoryViewWidget;
class UInventoryItem;

USTRUCT(BlueprintType)
struct FInventorySlot
{

	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FInventorySlotUpdate)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UInventoryItem> Item;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

	void BindOnInventorySlotUpdate(const FInventorySlotUpdate& Callback) const;
	void UnbindInventorySlotUpdate() const;
	
	void UpdateSlotState() const;
	void ClearSlot();

private:
	mutable FInventorySlotUpdate OnInventorySlotUpdate;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROAMBITIONS_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	void OpenViewInventory(APlayerController* PlayerController);
	void CloseViewInventory() const;
	bool IsViewVisible() const;

	int32 GetCapacity() const;
	bool HasFreeSlot() const;

	bool AddItem(TWeakObjectPtr<UInventoryItem> ItemToAdd, int32 Count);
	bool RemoveItem(FName ItemID);

	TArray<FInventorySlot> GetAllItemCopy() const;
	TArray<FText> GetAllItemsNames() const;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(EditAnywhere, Category = "Items")
	TSubclassOf<UInventoryViewWidget> InventoryViewWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	int32 Capacity = 4;

	void CreateViewWidget(APlayerController* PlayerController);

	FInventorySlot* FindItemSlot(FName ItemID);

	FInventorySlot* FindFreeSlot();
	
private:

	UPROPERTY(EditAnywhere, Category = "Items")
	UInventoryViewWidget* InventoryViewWidget;

	int32 ItemsInInventory;
};
