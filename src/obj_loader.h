#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <glm/glm.hpp>
#include <vector>

bool load_obj(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs, 
	std::vector<glm::vec3> & out_normals
);

#endif