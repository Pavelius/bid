#pragma once

const unsigned turns_per_day = 144; // One turn is 10 minutes
const unsigned days_per_year = 375;

unsigned day_of_year(unsigned v);
unsigned turns_diff(unsigned v1, unsigned v2);
unsigned year_of(unsigned v);
unsigned year_start(unsigned v);