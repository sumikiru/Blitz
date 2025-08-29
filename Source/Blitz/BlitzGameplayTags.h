// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "NativeGameplayTags.h"

namespace BlitzGameplayTags
{
	BLITZ_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all the custom native tags that Lyra will use
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	// Native Input
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Aim);

	// Ability
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_EquipWeapon_Rifle);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_EquipWeapon_Pistol);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_EquipWeapon_Unarmed);	// UnequipWeapon
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_FireGun);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_WeaponReload);
	
	// Ability Input
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Ability_EquipWeapon_Rifle);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Ability_EquipWeapon_Pistol);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Ability_EquipWeapon_Unarmed);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Ability_FireGun);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Ability_WeaponReload);

	// Status
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_EquippingWeapon_Rifle);
	BLITZ_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_EquippingWeapon_Pistol);
};
