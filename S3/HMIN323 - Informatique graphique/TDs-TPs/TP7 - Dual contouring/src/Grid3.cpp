#include "Grid3.hpp"

#include <limits>
#include <cassert>

Grid3::Grid3() : min(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()), max(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), resolution(1) {}
Grid3::Grid3(Vec3 min, Vec3 max, size_t resolution) : min(min), max(max), resolution(resolution) {}

Vec3 Grid3::max_top_left()
{
	return Vec3(min[0], max[1], max[2]);
}

Vec3 Grid3::max_top_right()
{
	return max;
}

Vec3 Grid3::max_bottom_left()
{
	return Vec3(min[0], min[1], max[2]);
}

Vec3 Grid3::max_bottom_right()
{
	return Vec3(max[0], min[1], max[2]);
}

Vec3 Grid3::min_top_left()
{
	return Vec3(min[0], max[1], min[2]);
}

Vec3 Grid3::min_top_right()
{
	return Vec3(max[0], max[1], min[2]);
}

Vec3 Grid3::min_bottom_left()
{
	return Vec3(min[0], min[1], min[2]);

}

Vec3 Grid3::min_bottom_right()
{
	return Vec3(max[0], min[1], min[2]);

}

Vec3 Grid3::world_to_grid_position(const Vec3& point)
{
	float grid_cell_size = (this->max_top_left() - this->max_top_right()).length();
	Vec3 grid_pos = (point - this->max_top_left()) / grid_cell_size;
	
	grid_pos[0] = floor(grid_pos[0]);
	grid_pos[1] = floor(grid_pos[1]);
	grid_pos[2] = floor(grid_pos[2]);

	return grid_pos;
}

// void Grid3::draw2d(size_t depth);
std::vector<Vec3 > Grid3::get_vertices()
{
	float grid_cell_size = (this->max_top_left() - this->max_top_right()).length();

	assert(grid_cell_size != 0);

	const Vec3 down(0, -1, 0);
	const Vec3 right(1, 0, 0);

	std::vector<Vec3> vertices((resolution+1) * (resolution+1) * (resolution+1));

	for (size_t depth = 0; depth <= resolution; ++depth)
	{
		// std::cerr << "drawing rows on depth " << depth << "...\n";
		for (size_t i = 0; i <= resolution; ++i)
		{
			Vec3 start = this->max_top_left() + down * grid_cell_size * (float)i;
			Vec3 end = start + right * grid_cell_size * (float)resolution;
			// std::cerr << i << " : from " << start << " to " << end <<"\n";
			vertices.push_back(start);
			vertices.push_back(end);
			// draw_line(start, end);
		}

		// std::cerr << "drawing column on depth " << depth << "...\n";
		for (size_t j = 0 ; j <= resolution; ++j)
		{
			Vec3 start = this->max_top_left() + right * grid_cell_size * (float)j;
			Vec3 end = start + down * grid_cell_size * (float)resolution;
			// std::cerr << j << " : from " << start << " to " << end <<"\n";
			vertices.push_back(start);
			vertices.push_back(end);
			// draw_line(start, end);
		}
	}

	return vertices;
}

