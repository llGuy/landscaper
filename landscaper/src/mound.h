#pragma once

#include <vector>
#include "types.h"
#include "detail.h"
#include <glm/glm.hpp>

struct mound_point
{
	union { glm::ivec2 diff_from_center, coord; };
	f32 quotient;
};

/* mound of a certain radius, class only contains one quarter of circle*/
template <u32 R> class mound_quarter_prototype
{
public:
	mound_quarter_prototype(void)
	{
		for (uint32_t z = 0; z < R; ++z)
		{
			for (uint32_t x = 0; x < R; ++x)
			{
				float radf = static_cast<float>(R);
				float xf = static_cast<float>(x);
				float zf = static_cast<float>(z);
				float distance = glm::distance(glm::vec2(xf, zf), glm::vec2(0.0f));
				if (distance <= radf || detail::fequ(distance, radf))
				{
					points.push_back(mound_point{ glm::ivec2(x, z), std::cos((distance / radf) * 1.5f) });
				}
			}
		}
	}
	auto size(void) const -> u32 { return points.size(); };
	auto operator[](u32 i) -> mound_point & { return points[i]; };

	auto at(u32 i, glm::ivec2 const & point, i32 xquot, i32 zquot) -> mound_point
	{
		mound_point mp = points[i];
		mp.coord.x *= xquot;
		mp.coord.y *= zquot;
		mp.coord += point;

		return mp;
	}
private:
	std::vector<mound_point> points;
};

namespace {

	constexpr u32 default_mound_size = 20;

}

using default_mound = mound_quarter_prototype<default_mound_size>;