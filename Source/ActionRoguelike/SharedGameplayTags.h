#pragma once
#include "NativeGameplayTags.h"


namespace SharedGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Health);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_HealthMax);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_MoveSpeed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_MoveSpeedMultiplier);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Rage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_RageMax);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_MagicAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_BlackholeAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Action_BlinkAttack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusEffect_Sprinting);
}
