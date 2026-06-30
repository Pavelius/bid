#include "answers.h"
#include "bsdata.h"
#include "character.h"
#include "collection.h"
#include "message.h"
#include "stringvar.h"

static unsigned char choose(collection& source, const char** names, const char* title, const char* cancel) {
	an.clear();
	for(auto v : source)
		an.add(v, names[v]);
	return (unsigned char)an.choose(title, cancel);
}

unsigned char choosev(unsigned long t1, unsigned long t2, const char** names, const char* title = 0, const char* cancel = 0) {
	an.clear();
	for(auto i=t1; i<t2; i++)
		an.add(i, names[i]);
	an.sort();
	return (unsigned char)an.choose(title, cancel);
}

template<class T> T choose(T t2, messagen title) {
	return (T)choose_enum(0, t2, bsenum<T>::names, getname(title), 0);
}

void game_run() {
	add_players();
}

static void player_name(stringbuilder& sb) {

}

BSDATA(stringvari) = {
	{"Player", player_name},
};
BSDATAF(stringvari)