#pragma once

enum messagen : unsigned char {
	GameTitle,
	PlayerCharged,
	PlayerCriticalMiss, PlayerMiss, PlayerHit, PlayerCriticalHit, MsgDamage, MsgDamageAndDead,
	PlayerStunned, PlayerSufferStarvation,
	PlayerTreatedIllness, PlayerTreatedWounds,
	PlayerForageItem, PlayerHuntingGame,
	PlayerRepairGear, WeaponBroken, WeaponDamage,
	PlayerMemorizeSpells,
	PartyMakeCamp,
	MakeCampInSafeCave, MakeCampInOpenLand,
	AdventureNextDay, PlayerHearNoiseOnWatch,
	PlayerThink, PlayerSay, PlayerCry,
	SayWhoIsHere,
	AskLeft, AskMemorizeSpells, AskWhatToDo,
};