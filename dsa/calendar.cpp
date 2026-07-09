#include "calendar.h"

/////////////////////////////////////
// CALCULATION
// Time measure in turns. Each turn is 10 minutes.
// One day is 24*6 = 144 turns.

unsigned year_of(unsigned v) {
	return v / (days_per_year * turns_per_day);
}

unsigned year_start(unsigned v) {
	return (v / (days_per_year * turns_per_day)) * (days_per_year * turns_per_day);
}

unsigned day_of_year(unsigned v) {
	return (v / turns_per_day) % days_per_year;
}

unsigned turns_diff(unsigned v1, unsigned v2) {
	return (v1 > v2) ? v1 - v2 : v2 - v1;
}