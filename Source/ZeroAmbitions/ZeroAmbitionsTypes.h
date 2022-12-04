#pragma once

#define ECC_InteractionVolume ECC_GameTraceChannel1
#define ECC_Bullet ECC_GameTraceChannel2

const FName FXParamTraceEnd = FName("TraceEnd");

const FName SocketWeaponForeGrip = FName("ForeGripSocket");
const FName SocketFPCamera = FName("CameraSocket");
const FName SocketBarellMuzzle = FName("MuzzleSocket");
const FName SocketCharacterWeapon = FName("CharacterWeaponSocket");

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfileRagdoll = FName("Ragdoll");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");

const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");
const FName DebugCategoryRangeWeapon = FName("RangeWeapon");

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	None,
	Pistol,
	Rifle
};
