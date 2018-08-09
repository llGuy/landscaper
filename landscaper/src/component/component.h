#pragma once

#include <array>
#include "../types.h"

struct comp_base { virtual auto update(void) -> void = 0; };

template <typename T> struct component : comp_base
{
	T value; entity * bound;
	/* stuff like position and direction don't need updating */
	auto update(void) -> void override {  };
};

struct entity 
{ 
	static constexpr u32 max_components = 10;

	std::array<comp_base *, max_components> components;

	pos_3D pos;
	dir_3D dir;
	dir_3D vel;

	u32 comp_count;
	u32 id; 
};

template <typename T, u32 M> class system
{
public:
	using type = system<T, M>;

	system(void) : counter(0) {};
	// returns back of the list of components
	auto back(void) -> T & { return list[counter].value; };
	// simulates array::operator[]
	auto operator[](u32 i) -> T & { list[i].value; };
	auto count(void) -> u32 & { return counter; };
private:
	std::array<component<T>, M> list;
	u32 counter;
};

template <typename C, typename S> auto add_component(C && comp, S && sys, entity & ent) -> void
{
	sys.back() = comp;
	sys.back().bound = &ent;

	if (ent.comp_count < entity::max_components)
		ent.components[ent.comp_count++] = &sys.back();

	++sys;
}