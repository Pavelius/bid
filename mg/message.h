#pragma once

enum messagen : unsigned char {
	AppTitle,
	ChooseGuardRang, ChooseBirthPlace, ChooseBirthPlaceSkill,
	LastMessage = ChooseBirthPlaceSkill,
};

extern const char* message_names[LastMessage + 1];
