#ifndef GRID3_HPP
#define GRID3_HPP

#include <vector>

#include "Vec3.h"

auto Vec3_inf = [](const Vec3& left, const Vec3& right)
{
	if (left[0] < right[0])
	{
		return true;
	}
	else if (left[0] == right[0])
	{
		if (left[1] < right[1])
		{
			return true;
		}
		else if (left[1] == right[1])
		{
			if (left[2] < right[2])
			{
				return true;
			}
		}
	}
	
	return false;
};

struct Grid3
{
	Vec3 min;
	Vec3 max;
	size_t resolution;

	Grid3();
	Grid3(const Vec3& min, const Vec3& max, size_t resolution);

	// max en fonction des z
	Vec3 max_top_left();
	Vec3 max_top_right();
	Vec3 max_bottom_left();
	Vec3 max_bottom_right();

	// min en fonction des z
	Vec3 min_top_left();
	Vec3 min_top_right();
	Vec3 min_bottom_left();
	Vec3 min_bottom_right();

	Vec3 world_to_grid_position(const Vec3& world_point);

	std::vector<Vec3> get_vertices();
};

#endif 
