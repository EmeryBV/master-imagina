#include "../include/mesh.h"

template<typename VERTEX_TYPE, typename INDICE_TYPE>
Mesh<VERTEX_TYPE, INDICE_TYPE>::Mesh() : vertexes(), vertexes_indices() {}

template<typename VERTEX_TYPE, typename INDICE_TYPE>
Mesh<VERTEX_TYPE, INDICE_TYPE>::Mesh(const std::string& filename) {}