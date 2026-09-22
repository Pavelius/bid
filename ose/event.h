#pragma once

enum abilityn : unsigned char;
enum messagen : unsigned char;
enum picturen : unsigned char;

typedef void(*fnevent)();
typedef bool(*fncondition)();

enum eventf : unsigned char {
	StartEvent, InfiniteEvent,
};

struct eventfc {
	unsigned char data;
	constexpr eventfc() : data{0} {}
	template<typename... Ts> constexpr eventfc(eventf v, Ts... args) : eventfc(args...) { set(v); }
	constexpr bool is(eventf v) const { return (data & (1 << v)) != 0; }
	constexpr void set(eventf v) { data |= 1 << v; }
};

struct eventi {
	picturen	picture;
	messagen	message;
	fncondition	condition;
	eventfc		flags;
	abilityn	ability;
	char		bonus;
	fnevent		success, fail;
	int index() const;
	bool allow() const { return condition ? condition() : true; }
	bool is(eventf v) const { return flags.is(v); }
	bool play() const; // Extern function realization.
};
extern eventi events[];
extern eventi* last_event;