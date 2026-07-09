#include "gender.h"
#include "stringbuilder.h"

bool apply_gender(const char* identifier, stringbuilder& sb, gendern gender) {
	struct gender_change_string {
		const char*	female;
		const char*	male;
		const char*	multiply;
	};
	static gender_change_string player_gender[] = {
		{"госпожа", "господин", "господа"},
		{"женщина", "мужчина", "господа"},
		{"стерва", "ублюдок", "ублюдки"},
		{"миледи", "милорд", "милорды"},
		{"такая", "такой", "такие"},
		{"леди", "лорд", "лорды"},
		{"ась", "ся", "ись"},
		{"нее", "него", "них"},
		{"она", "он", "они"},
		{"шла", "шел", "шли"},
		{"ая", "ый", "ые"},
		{"ее", "его", "их"},
		{"ей", "ему", "им"},
		{"ла", "", "ли"},
		{"а", "", "и"},
	};
	for(auto& e : player_gender) {
		if(!equal(e.female, identifier))
			continue;
		if(gender == NoGender)
			sb.add(e.multiply);
		else if(gender == Female)
			sb.add(e.female);
		else
			sb.add(e.male);
		return true;
	}
	return false;
}
