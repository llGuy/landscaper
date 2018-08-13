#pragma once

#include <new>
#include <algorithm>
#include <array>
#include <tuple>
#include <functional>
#include "../types.h"

struct comp_base { virtual auto update(f32 td) -> void = 0; };

struct entity_data 
{ 
	pos_3D pos; dir_3D dir; dir_3D vel; 
	f32 speed; f32 height; 
	bool at_ground_height { false }; 

	f32 max_walk_speed;
};

struct entity 
{ 
	static constexpr u32 max_components = 5;

	std::array<comp_base *, max_components> components{ 0 };

	entity_data data;

	u32 comp_count;
	u32 id;
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

	comp_system(void) : counter(0), list{}
	{
	}

	auto update(u32 start, f32 td) -> void
	{
		for (u32 i = start; i < counter; ++i)
		{
			list[i].update(td);
		}
	}

	/* callable must take a pointer to component */
	template <typename C, typename ... T> auto update(f32 td, C && callable, T && ... params) -> void
	{
		for (u32 i = 0; i < counter; ++i)
		{
			callable(&list[i], params...);
		}
	}

	/* returns back of the list of components */
	auto back(void) -> component<T> & 
	{
		return list[counter]; 
	}
	/* simulates array::operator[] */
	auto operator[](u32 i) -> component<T> & 
	{ 
		return list[i]; 
	}
	auto count(void) -> u32 & 
	{ 
		return counter; 
	}
private:
	std::array<component<T>, M> list;
	u32 counter;
};

template <typename C, typename S, typename ... Ps> auto add_component(S && sys, entity & ent, Ps && ... params) -> void
{
	new(&sys.back()) component<C>(std::forward<Ps>(params)...);
	sys.back().bound = &ent;

	if (ent.comp_count < entity::max_components)
		ent.components[ent.comp_count++] = &sys.back();

	++(sys.count());
}

/* compile time component systems tests */
template <typename ... T> struct const_entity 
{
	std::tuple <component<T>...> components;

	template <typename T> auto get(void) 
	{
		return std::get<component<T>>(components);
	}

	auto component_count(void) -> u32 
	{
		return std::tuple_size<std::tuple<component<T>...>>::value;
	}
};