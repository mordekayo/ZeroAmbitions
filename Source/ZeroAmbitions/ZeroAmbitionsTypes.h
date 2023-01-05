#pragma once

#define ECC_Melee ECC_GameTraceChannel1
#define ECC_InteractionVolume ECC_GameTraceChannel2
#define ECC_Bullet ECC_GameTraceChannel4

const FName FXParamTraceEnd = FName("TraceEnd");

const FName SocketWeaponForeGrip = FName("ForeGripSocket");
const FName SocketFPCamera = FName("CameraSocket");
const FName SocketBarellMuzzle = FName("MuzzleSocket");
const FName SocketCharacterWeapon = FName("CharacterWeaponSocket");

const FName CollisionProfileNoCollision = FName("NoCollision");
const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfileRagdoll = FName("Ragdoll");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");

const FName DebugCategoryCharacterAttributes = FName("CharacterAttributes");
const FName DebugCategoryRangeWeapon = FName("RangeWeapon");
const FName DebugCategoryMeleeWeapon = FName("MeleeWeapon");

const FName SectionMontageReloadEnd = FName("ReloadEnd");

const FName ActionInteract = FName("Interact");

const FName BB_CurrentTarget = FName("CurrentTarget");
const FName BB_NextLocation = FName("NextLocation");

UENUM(BlueprintType)
enum class EEquipableItemType : uint8
{
	None,
	Pistol,
	Rifle,
	Throwable,
	Melee
};

UENUM(BlueprintType)
enum class EAmmunitionType	 : uint8
{
	None,
	Pistol,
	Rifle,
	ShotgunShells,
	MAX UMETA(Hidden)
};


UENUM(BlueprintType)
enum class EEquipmentSlots : uint8
{
	None,
	MeleeWeapon,
	SideArm,
	PrimaryWeapon,
	SecondaryWeapon,
	PrimaryItemSlot,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ETeams : uint8
{
	Player,
	Enemy
};


UENUM(BlueprintType)
enum class EMeleeAttackTypes : uint8
{
	None,
	PrimaryAttack,
	SecondaryAttack,
	MAX UMETA(Hidden)
};