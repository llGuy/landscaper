#pragma once


#include <array>
#include <glm/glm.hpp>
#include <type_traits>
#include <algorithm>

namespace detail {

	template <typename T1, typename T2, typename ... Args> auto construct(Args ... params) -> T1
	{
		return T1(static_cast<T2>(params)...);
	}

	template <typename ... T> auto make_array(T && ... params) -> std::array <typename std::common_type <T...>::type, sizeof... (T)>
	{
		return { std::forward<T>(params)... };
	}

	template <typename T, u32 N> auto vec_rand(i32 max) -> glm::vec<N, T, glm::highp>
	{
		using type = glm::vec<N, T, glm::highp>;
		type result;
		for (u32 i = 0; i < N; ++i) result[i] = static_cast<T>(rand() % max);
		return result;
	}

	namespace {

		glm::mat4 identity_matrix(1);
		glm::vec4 null_vector(0);
		glm::vec3 up(0, 1, 0);

	}

}