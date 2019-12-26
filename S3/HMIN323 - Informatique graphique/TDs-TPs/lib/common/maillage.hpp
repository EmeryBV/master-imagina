#ifndef __MAILLAGE_H__
#define __MAILLAGE_H__

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "objloader.hpp"

void draw_line(const glm::vec3& start, const glm::vec3& end);

struct Grid3
{
	glm::vec3 min;
	glm::vec3 max;
	size_t resolution;

	Grid3();
	Grid3(glm::vec3 min, glm::vec3 max, size_t resolution);

	// max en fonction des z
	glm::vec3 max_top_left();
	glm::vec3 max_top_right();
	glm::vec3 max_bottom_left();
	glm::vec3 max_bottom_right();

	// min en fonction des z
	glm::vec3 min_top_left();
	glm::vec3 min_top_right();
	glm::vec3 min_bottom_left();
	glm::vec3 min_bottom_right();

	glm::vec3 world_to_grid_position(const glm::vec3& world_point);

	std::vector<glm::vec3> get_vertices();
};

class Maillage
{
public:

	Maillage(const std::string& filename);

	// Méthodes

	void simplify(size_t resolution);
	
	glm::vec3 min();
	glm::vec3 max();

	// Attributs

	std::vector< glm::vec3 > vertices;
    std::vector< unsigned short > indices;
    std::vector< std::vector<unsigned short > > triangles;
};

#endif 
