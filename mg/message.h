#pragma once

enum messagen : unsigned char {
	AppTitle,
	ChooseGuardRang,
	LastMessage = ChooseGuardRang,
};

extern const char* message_names[LastMessage + 1];
