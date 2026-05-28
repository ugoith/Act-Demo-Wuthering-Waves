#pragma once
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Tests
{
	namespace SetByCaller
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tests_SetByCaller_Test);
		
	}
} 
namespace Appearance
{
	namespace Decorations
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Appearance_Decorations_1);
	}
	namespace Weapons
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Appearance_Weapons_1);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Appearance_Weapons_2);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Appearance_Weapons_3);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Appearance_Weapons_4);
	}

	namespace Bodies
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Appearance_Bodies_1);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Appearance_Bodies_2)
	}
}
namespace InputActions
{
	namespace Move
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_Move);
	}
	namespace Jump
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_Jump);
	}
	namespace Look
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_Look);
	}
	namespace MeleeAttack
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_MeleeAttack);
	}
	namespace Dodge
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_Dodge);
	}
	namespace Skill
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_Skill);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_UltimateSkill);
	}
	namespace TriggerEvents
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_TriggerEvents_Started);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_TriggerEvents_Trigger);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_TriggerEvents_Ongoing);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_TriggerEvents_Canceled);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputActions_TriggerEvents_Completed);
	}
}
namespace BattleActions
{
	namespace Interrupt
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleActions_Interrupt_CanMove);
	}
	namespace ArmorState
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleActions_ArmorState_Armor);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleActions_ArmorState_Invincible);
	}
	namespace BeHit
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleActions_BeHit);
	}
}
namespace AI
{
	namespace Actions
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Actions_Patrol);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Actions_Chase);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Actions_Battle);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Actions_BeHit);
	}
}
namespace EventTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTags_HitSuccess);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTags_Ability_End);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTags_SliceSuccess);
}