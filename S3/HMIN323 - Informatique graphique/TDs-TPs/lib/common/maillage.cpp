#include "maillage.hpp"

#include <GL/glut.h>
#include <algorithm>
#include <iostream>
#include <map>


std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
	return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

// bool operator<(const glm::ivec3& left, const glm::ivec3& right)
// {
// 	if (left.x < right.x)
// 	{
// 		return true;
// 	}
// 	else if (left.x == right.x)
// 	{
// 		if (left.y < right.y)
// 		{
// 			return true;
// 		}
// 		else if (left.y == right.y)
// 		{
// 			if (left.z < right.z)
// 			{
// 				return true;
// 			}
// 		}
// 	}
	
// 	return false;
// }

// operator< ne marche pas pour une raison que je ne comprend pas.
auto inf = [](glm::vec3 left, glm::vec3 right)
{
	if (left.x < right.x)
	{
		return true;
	}
	else if (left.x == right.x)
	{
		if (left.y < right.y)
		{
			return true;
		}
		else if (left.y == right.y)
		{
			if (left.z < right.z)
			{
				return true;
			}
		}
	}
	
	return false;
};

void draw_line(const glm::vec3& start, const glm::vec3& end)
{
  glBegin(GL_LINES);
    glVertex3d(start.x, start.y, start.z); glColor3d(1, 0, 0);
    glVertex3d(end.x, end.y, end.z);       glColor3d(1, 0, 0);
  glEnd();
}

Grid3::Grid3() : min(), max(), resolution() {}
Grid3::Grid3(glm::vec3 min, glm::vec3 max, size_t resolution) : min(min), max(max), resolution(resolution) {}

glm::vec3 Grid3::max_top_left()
{
	return glm::vec3(min.x, max.y, max.z);
}

glm::vec3 Grid3::max_top_right()
{
	return max;
}

glm::vec3 Grid3::max_bottom_left()
{
	return glm::vec3(min.x, min.y, max.z);
}

glm::vec3 Grid3::max_bottom_right()
{
	return glm::vec3(max.x, min.y, max.z);
}

glm::vec3 Grid3::min_top_left()
{
	return glm::vec3(min.x, max.y, min.z);
}

glm::vec3 Grid3::min_top_right()
{
	return glm::vec3(max.x, max.y, min.z);
}

glm::vec3 Grid3::min_bottom_left()
{
	return glm::vec3(min.x, min.y, min.z);

}

glm::vec3 Grid3::min_bottom_right()
{
	return glm::vec3(max.x, min.y, min.z);

}

glm::vec3 Grid3::world_to_grid_position(const glm::vec3& point)
{
	float grid_cell_size = (this->max_top_left() - this->max_top_right()).length();
	glm::vec3 grid_pos = (point - this->max_top_left()) / grid_cell_size;
	
	grid_pos.x = floor(grid_pos.x);
	grid_pos.y = floor(grid_pos.y);
	grid_pos.z = floor(grid_pos.z);

	return grid_pos;
}

// void Grid3::draw2d(size_t depth);
std::vector<glm::vec3 > Grid3::get_vertices()
{
	float grid_cell_size = (this->max_top_left() - this->max_top_right()).length();

	assert(grid_cell_size != 0);

	const glm::vec3 down(0, -1, 0);
	const glm::vec3 right(1, 0, 0);

	std::vector<glm::vec3> vertices((resolution+1) * (resolution+1) * (resolution+1));

	for (size_t depth = 0; depth <= resolution; ++depth)
	{
		// std::cerr << "drawing rows on depth " << depth << "...\n";
		for (size_t i = 0; i <= resolution; ++i)
		{
			glm::vec3 start = this->max_top_left() + down * grid_cell_size * (float)i;
			glm::vec3 end = start + right * grid_cell_size * (float)resolution;
			// std::cerr << i << " : from " << start << " to " << end <<"\n";
			vertices.push_back(start);
			vertices.push_back(end);
			// draw_line(start, end);
		}

		// std::cerr << "drawing column on depth " << depth << "...\n";
		for (size_t j = 0 ; j <= resolution; ++j)
		{
			glm::vec3 start = this->max_top_left() + right * grid_cell_size * (float)j;
			glm::vec3 end = start + down * grid_cell_size * (float)resolution;
			// std::cerr << j << " : from " << start << " to " << end <<"\n";
			vertices.push_back(start);
			vertices.push_back(end);
			// draw_line(start, end);
		}
	}

	return vertices;
}

// Maillage

Maillage::Maillage(const std::string& filename)
{
	loadOFF(filename, this->vertices, this->indices, this->triangles);
}

void Maillage::simplify(size_t resolution)
{
	// offset pour enlever les problèmes d'impressisions
	Grid3 grid(this->min() - glm::vec3(0.1, 0.1, 0.1), this->max() + glm::vec3(0.1, 0.1, 0.1), resolution);

	// associe des sommets à leur positionnement dans la grille
	std::map<glm::vec3, std::vector<glm::vec3>, decltype(inf)> region_vertices(inf);

	for (auto& v : vertices)
	{
		region_vertices[grid.world_to_grid_position(v)].push_back(v);
	}

	// associe un unique representant pour les cellule de la grille contenant des sommets
	std::map<glm::vec3, glm::vec3, decltype(inf)> region_representant(inf);

	for (auto& rv : region_vertices)
	{
		glm::vec3 representant;

		for (auto& v : rv.second)
		{
			representant += v;
		}

		representant /= rv.second.size();
		
		region_representant[rv.first] = representant;
	}

	std::vector< std::vector<unsigned short > > triangles_survivants;

	indices.clear();
	vertices.clear();

	// réindexation du maillage
	for (auto& t : triangles)
	{
		// check si tout les points du triangles sont dans une région différente de la grille
		if (region_representant[vertices[t[0]]] != region_representant[vertices[t[1]]] && 
			region_representant[vertices[t[0]]] != region_representant[vertices[t[2]]])
		{
			triangles_survivants.push_back(t);
			indices.push_back(t[0]);
			indices.push_back(t[1]);
			indices.push_back(t[2]);
		}
	}

	// associe à chaque point sont représentant dans la grille.
	for (auto& v : vertices)
	{
		v = region_representant[v];
	}

	triangles = triangles_survivants;
}

glm::vec3 Maillage::min()
{
	// return glm::vec3();
	return *std::min_element(vertices.begin(), vertices.end(), inf);
}

glm::vec3 Maillage::max()
{
	// return glm::vec3();
	return *std::max_element(vertices.begin(), vertices.end(), inf);
}