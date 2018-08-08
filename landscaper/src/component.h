#pragma once

#include <array>
#include "types.h"

/* component types */
struct camera;

struct comp_base { virtual auto update(void) -> void = 0; };
template <typename T> struct component : comp_base
{
	T value; u32 id;
	/* stuff like position and direction don't need updating */
	auto update(void) -> void override {  };
};


struct entity { u32 id; };

template <typename T, u32 M> class system
{
public:
	using type = system<T, M>;

	system(void) : counter(0) {};
	// returns back of the list of components
	auto back(void) -> T & { return list[counter]; };
	// simulates array::operator[]
	auto operator[](u32 i) -> T & { list[i]; };
	auto count(void) -> u32 & { return counter; };
private:
	std::array<T, M> list;
	u32 counter;
};

template <typename C, typename S> auto add_component(C && comp, S && sys, entity & ent) -> void
{
	sys.back() = comp;
	sys.back().id = ent.id;
	++sys;
}