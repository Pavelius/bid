#pragma once

enum refidn : unsigned char {
	NoObject, Areas, Creatures,
};
struct refid {
	refidn			type;
	short unsigned	value;
	constexpr refid() : type(NoObject), value(0) {}
	constexpr refid(refidn type, short unsigned value) : type(type), value(value) {}
	constexpr bool operator==(const refid& v) const { return type == v.type && value == v.value; }
	constexpr explicit operator bool() const { return type != (refidn)0; }
	template<typename T> refid(const T* p);
};