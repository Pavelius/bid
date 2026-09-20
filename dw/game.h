#pragma once

enum messagen : unsigned char {
	AppTitle, Next,
	ChooseOption, ChooseOptionLeftCount,
	MsgReadyWeapon, MsgButSomethingHappen,
	MsgHackAndSlashHit, MsgEvadeEnemyAttack, MsgApplyAdditionalDamage,
	MsgVolleyHit, MsgVolleyUseAmmo, MsgVolleyWeak, MsgVolleyEnemyMove,
	LastMessage = MsgVolleyEnemyMove,
};

extern const char* message_names[LastMessage + 1];

extern itemfn scene_range;
extern int bonus_damage, bonus_enemy_damage;

inline const char* getname(messagen v) { return message_names[v]; }
