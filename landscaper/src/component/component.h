#pragma once

#include <new>
#include <algorithm>
#include <array>
#include "../types.h"

struct comp_base { virtual auto update(f32 td) -> void = 0; };

struct entity
{ 
	static constexpr u32 max_components = 5;
	std::array<comp_base *, max_components> components{ 0 };
	pos_3D pos;
	dir_3D dir;
	dir_3D vel;
	u32 comp_count;
	u32 id;

	auto update(f32 td) -> void
	{
		std::for_each(components.begin(), components.end(), 
			[&td](comp_base * component) { component->update(td); });
	}
};

template <typename T> struct component : comp_base
{
	T value; entity * bound;
	/* stuff like position and direction don't need updating */
	auto update(f32 td) -> void override {  };
};

template <typename T, u32 M> class comp_system
{
public:
	using type = comp_system<T, M>;

	comp_system(void) : counter(0), list{} {};
	// returns back of the list of components
	auto back(void) -> component<T> & { return list[counter]; };
	// simulates array::operator[]
	auto operator[](u32 i) -> component<T> & { list[i].value; };
	auto count(void) -> u32 & { return counter; };
private:
	std::array<component<T>, M> list;
	u32 counter;
};

template <typename C, typename S, typename ... Ps> auto add_component(S && sys, entity & ent, Ps && ... params) -> void
{
	new(&sys.back()) component<C>(std::forward<Ps>(params)...);
//	sys.back() = component<C>( std::forward<Ps>(params)... );
	sys.back().bound = &ent;

	if (ent.comp_count < entity::max_components)
		ent.components[ent.comp_count++] = &sys.back();

	++(sys.count());
}