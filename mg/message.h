#pragma once

enum messagen : unsigned char {
	AppTitle, Yes, No,
	ChooseGuardRang, ChooseBirthPlace, ChooseBirthPlaceSkill, ChooseBirthPlaceTrait, ChooseLifeExperience,
	ChooseParentProffession, ChooseConversationSkills, ChooseSeniorArtisanTeaching, ChooseMentorTeaching, ChooseYouSpeciality,
	DoYouFearPredators,
	ChooseTrait,
	LastMessage = ChooseTrait,
};

extern const char* message_names[LastMessage + 1];