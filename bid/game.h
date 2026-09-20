#pragma once

enum messagen : unsigned char {
	AppTitle,
	PageActions, PageItems,
	GainStress, ActionRollHeader, ActionRollHelp, MakeRoll,
	DevilsBagrainExpensive, DevilsBagrainBetrail, DevilsBagrainHarm,
	ChoosePlaybook, ChooseHeiretage, ChooseBackground, ChooseActionDot,
	LastMessage = ChooseActionDot,
};

extern const char* message_names[LastMessage + 1];

inline const char* getname(messagen v) { return message_names[v]; }