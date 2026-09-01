#pragma once

enum messagen : unsigned char {
	GameTitle,
	MsgAnd,
	CoinsPl, CoinsCP, CoinsSP, CoinsEP, CoinsGP, CoinsPP,
	PlayerCharged,
	PlayerCriticalMiss, PlayerMiss, PlayerHit, PlayerCriticalHit, MsgDamage, MsgDamageAndDead,
	PlayerStunned, PlayerSufferStarvation,
	PlayerTreatedIllness, PlayerTreatedWounds,
	PlayerForageItem, PlayerHuntingGame,
	PlayerRepairGear, WeaponBroken, WeaponDamage,
	PlayerMemorizeSpells,
	PlayerJumpFromBrush, PlayerJumpFromTree,
	PartyMakeCamp,
	MakeCampInSafeCave, MakeCampInOpenLand,
	AdventureNextDay, PlayerHearNoiseOnWatch,
	SearchBodies, NothingValuableHere,
	StateWounded,
	PlayerThink, PlayerSay, PlayerCry,
	SayWhoIsHere,
	AskLeft, AskMemorizeSpells, AskWhatToDo,
};