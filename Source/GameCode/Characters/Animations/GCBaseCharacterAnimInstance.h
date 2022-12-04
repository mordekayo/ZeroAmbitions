// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../../Components/WallRunComponent.h"
#include "GameCodeTypes.h"
#include "GCBaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMECODE_API UGCBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation", meta = (UIMin = 0.0f, UIMax = 500.0f))
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float OnLadderSpeedRatio = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOutOfStamina = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsCrouch = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSwimming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOnLadder = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsOnZipline = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsWallRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	EWallRunSide WallRunSide = EWallRunSide::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	EWallRunSide WallRunLeftSide = EWallRunSide::Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	EWallRunSide WallRunRightSide = EWallRunSide::Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSliding = false;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Settings | IK Settings")
	FVector IKLeftFootOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Settings | IK Settings")
	FVector IKRightFootOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "Settings | IK Settings")
	FVector IKPelvisBoneOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsStrafing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation", meta = (UIMin = -180.0f, UIMax = 180.0f))
	float Direction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character animation")
	EEquipableItemType CurrentEquippedItemType = EEquipableItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	FRotator AimRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation | Weapon")
	FTransform ForeGripSocketTransform;
private:
	TWeakObjectPtr<class AGCBaseCharacter> CachedBaseCharacter;
};
