// Copyright sumikiru. All Rights Reserved,

#include "BlitzGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "BlitzLogChannels.h"

namespace BlitzGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead",
	                               "Ability failed to activate because its owner is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown",
	                               "Ability failed to activate because it is on cool down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost",
	                               "Ability failed to activate because it did not pass the cost checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked",
	                               "Ability failed to activate because tags are blocking it.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing",
	                               "Ability failed to activate because tags are missing.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking",
	                               "Ability failed to activate because it did not pass the network checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup",
	                               "Ability failed to activate because of its activation group.");

	// Native Input
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Aim, "InputTag.Aim");

	// Ability
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Ability_EquipWeapon_Rifle, "Ability.EquipWeapon.Rifle");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Ability_EquipWeapon_Pistol, "Ability.EquipWeapon.Pistol");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Ability_EquipWeapon_Unarmed, "Ability.EquipWeapon.Unarmed");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Ability_FireGun, "Ability.FireGun");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Ability_WeaponReload, "Ability.WeaponReload");
	
	// Ability Input
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_EquipWeapon_Rifle, "InputTag.Ability.EquipWeapon.Rifle");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_EquipWeapon_Pistol, "InputTag.Ability.EquipWeapon.Pistol");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_EquipWeapon_Unarmed, "InputTag.Ability.EquipWeapon.Unarmed");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_FireGun, "InputTag.Ability.FireGun");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_WeaponReload, "InputTag.Ability.WeaponReload");

	// Status
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Status_EquippingWeapon_Rifle, "Status.EquippingWeapon.Rifle");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Status_EquippingWeapon_Pistol, "Status.EquippingWeapon.Pistol");
	BLITZ_API UE_DEFINE_GAMEPLAY_TAG(Status_AbilityInputBlocked, "Status.AbilityInputBlocked");

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogBlitz, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."),
					       *TagString,
					       *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
}
