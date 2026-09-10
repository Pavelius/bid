/*
	Copyright 2026 by Pavel Chistyakov

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.77
*/

#pragma once

enum messagen : unsigned char {
	NoMessage, Continue, Cancel, Yes, No, And,
	Passed, Failed,
	PageCharacter, PageSkills, PageItems, PageCombatants,
	ChooseName, ChooseWises, ChooseFurColor, ChooseCloackColor,
	ChooseGuardRang, ChooseBirthPlace, ChooseBirthPlaceSkill, ChooseBirthPlaceTrait, ChooseLifeExperience,
	ChooseParentProffession, ChooseConversationSkills, ChooseSeniorArtisanTeaching, ChooseMentorTeaching, ChooseYouSpeciality,
	MsgMakeRoll, MsgVsDifficult, MsgNumberDicesRoll, AskCanHelp, AskCanHelpWise, MakeRoll,
	DoYouSaveForWinter, DoYouStandGroundAndFight, DoYouFearPredators,
	ChooseTrait,
	LastMessage = ChooseTrait,
};

extern const char* message_names[LastMessage + 1];