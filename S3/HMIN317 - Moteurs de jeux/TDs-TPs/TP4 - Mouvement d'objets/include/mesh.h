#ifndef MESH_H 
#define MESH_H 

#include <vector>
#include <string>
#include <optional>

template<typename VERTEX_TYPE, typename INDICE_TYPE = unsigned int>
struct Mesh 
{
	using vertex_type = VERTEX_TYPE;
	using indice_type = INDICE_TYPE;

	std::vector<VERTEX_TYPE> vertexes;
	// std::optional<std::vector<INDICE_TYPE>> vertexes_indices;
	
	Mesh();
	Mesh(const std::string& filename);
};

// #include "../src/mesh.tpp";

#endif
