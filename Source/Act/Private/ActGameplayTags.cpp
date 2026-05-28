#include "ActGameplayTags.h"

namespace Tests
{
	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tests_SetByCaller_Test,"Tests.SetByCaller.Test","Only For Testing SetByCaller");
	}
}
namespace Appearance
{
	namespace Decorations
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Appearance_Decorations_1,"Appearance.Decorations.1","装饰品1");
	}
	namespace Weapons
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Appearance_Weapons_1,"Appearance.Weapons.1","武器1");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Appearance_Weapons_2,"Appearance.Weapons.2","武器2");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Appearance_Weapons_3,"Appearance.Weapons.3","武器3");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Appearance_Weapons_4,"Appearance.Weapons.4","武器4");
	}

	namespace Bodies
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Appearance_Bodies_1,"Appearance.Bodies.1","身体1");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Appearance_Bodies_2,"Appearance.Bodies.2","身体2")
	}
	
}
namespace InputActions
{
	namespace Move
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_Move,"InputActions.Move","移动");
	}
	namespace Jump
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_Jump,"InputActions.Jump","跳跃");
	}
	namespace Look
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_Look,"InputActions.Look","视角");
	}
	namespace MeleeAttack
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_MeleeAttack,"InputActions.MeleeAttack","近战攻击");
	}
	namespace Dodge
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_Dodge,"InputActions.Dodge","闪避");
	}
	namespace Skill
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_Skill,"InputActions.Skill","普通技能");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_UltimateSkill,"InputActions.UltimateSkill","大招");
	}
	namespace TriggerEvents
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_TriggerEvents_Started,"InputActions.TriggerEvents.Started","点击事件");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_TriggerEvents_Trigger,"InputActions.TriggerEvents.Trigger","触发");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_TriggerEvents_Ongoing,"InputActions.TriggerEvents.Ongoing","正在进行");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_TriggerEvents_Canceled,"InputActions.TriggerEvents.Canceled","被取消");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputActions_TriggerEvents_Completed,"InputActions.TriggerEvents.Completed","完成");
	}
}
namespace BattleActions
{
	namespace Interrupt
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(BattleActions_Interrupt_CanMove,"BattleActions.Interrupt.CanMove","允许在被打断时移动");
		
	}
	namespace ArmorState
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(BattleActions_ArmorState_Armor,"BattleActions.ArmorState.Armor","保持此姿态");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(BattleActions_ArmorState_Invincible,"BattleActions.ArmorState.Invincible","无敌");
	}
	namespace BeHit
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(BattleActions_BeHit,"BattleActions.BeHit","被击（被判定为需要播放被击动画，会加入此标签）")
	}
}
namespace AI
{
	namespace Actions
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Actions_Patrol,"AI.Actions.Patrol","巡逻");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Actions_Chase,"AI.Actions.Chase","追击跟随等");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Actions_Battle,"AI.Actions.Battle","战斗状态");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Actions_BeHit,"AI.Actions.BeHit","受击被控，且无法行动的状态");
		
	}
}
namespace EventTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTags_HitSuccess,"EventTags.HitSuccess","碰撞检测命中事件");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTags_Ability_End,"EventTags.Ability.End","碰撞检测命中事件");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTags_SliceSuccess,"EventTags.SliceSuccess","切割物体成功");
	
}