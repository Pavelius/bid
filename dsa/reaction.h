#pragma once

enum reactionn : unsigned char {
	Hostile, Unfriendly, Neutral, Indifferent, Friendly,
};
extern reactionn last_reaction;

void make_reaction_roll(int bonus);
