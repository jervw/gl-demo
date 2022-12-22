#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>


class Mesh  
{
public:
	std::string m_file;
	Mesh(std::string file, GLuint shader);
	~Mesh();
	void clear();
	void draw_mesh();
	void translate(glm::vec3 position);
 
private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	GLuint m_vbo;
	GLuint m_vcb;
	GLuint m_vao;
	GLuint m_modelMatrixID;
	glm::mat4 m_translateMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_modelMatrix;
	GLuint m_shader;
 
	void load();
};

#endif