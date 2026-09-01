#pragma once

#include "stringbuilder.h"

struct sprite;

enum messagen : unsigned char;

typedef bool(*fnvisible)(const void* object);
typedef void(*fnevent)();
typedef int(*fngetnum)(const void* object);

namespace metrics {
extern sprite* avatars;
extern sprite* images;
}

extern stringbuilder sb;
extern void* current_avatar;
extern long current_avatar_post;
extern fnevent atg_menu;

void* choose_record(const char* id, const char* source_id, fnvisible allow = 0);

bool allow_paint();
void fixmsg(messagen id);
void game_run(); // Main game entry point
void main_util(); // External function
void next_scene(fnevent v);
void paint_avatars(void** source, int count, fngetnum getavatar, void* current_player, fngetnum gethits);
void paint_bar(const char* name, fnevent proc);
void paint_button(const char* format, long param, bool choose = false, int padding = -1);
void paint_hilite();
void paint_separator();
void paint_status_bar();
void paint_status_text();
void paint_window_center(const char* format);
void paint_window_info(const char* format);
bool running_scene();
void set_hilite_state(const char* format);
void stringbuilder_custom(stringbuilder& sb, const char* id);