#include "bsdata.h"
#include "game.h"

bool is_import(itemn resource, govermentn v) {
	switch(v) {
	case Anarchy:
		switch(resource) {
		case Weapons: case Techincs: return true;
		default: break;
		}
	case Monarchy:
		switch(resource) {
		case Minerals: case Drugs: return true;
		default: break;
		}
	default: break;
	}
	return false;
}

bool is_export(itemn resource, govermentn v) {
	switch(v) {
	case Anarchy:
		switch(resource) {
		case Drugs: return true;
		default: break;
		}
	case Monarchy:
		switch(resource) {
		case Weapons: case Drugs: return true;
		default: break;
		}
	default: break;
	}
	return false;
}

bool is_illegal(itemn resource, govermentn v) {
	switch(v) {
	case Democraty:
		switch(resource) {
		case Weapons: case Drugs: case Alcohole: return true;
		default: break;
		}
	case Monarchy:
		switch(resource) {
		case Alcohole: case Drugs: return true;
		default: break;
		}
	default: break;
	}
	return false;
}