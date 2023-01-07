// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWheelWidget.generated.h"

enum class EEquipmentSlots : uint8;
struct FWeaponTableRow;
class UTextBlock;
class UImage;
class UCharacterEquipmentComponent;
/**
 * 
 */
UCLASS()
class ZEROAMBITIONS_API UWeaponWheelWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void InitializeWeaponWheelWidget(UCharacterEquipmentComponent* EquipmentComponent);

	void NextSegment();
	void PrevSegment();
	void ConfirmSelection();
	
protected:

	virtual void NativeConstruct() override;
	void SelectSegment();

	UPROPERTY(meta = (BindWidget))
	UImage* RadialBackground;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponNameText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon wheel settings")
	TArray<EEquipmentSlots> EquipmentSlotsSegments;

private:

	FWeaponTableRow* GetTableRowForSegment(int32 SegmentIndex);
	
	int32 CurrentSegmentIndex;
	UMaterialInstanceDynamic* BackgroundMaterial;
	TWeakObjectPtr<UCharacterEquipmentComponent> LinkedEquipmentComponent;
};
