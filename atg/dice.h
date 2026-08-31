#pragma once

struct dice {
	char c, d, b, m;
	constexpr explicit operator bool() const { return c || b; }
	constexpr int maximum() const { return d ? c * d + b : c; }
	constexpr int minimum() const { return c + b; }
	int	roll() const;
};