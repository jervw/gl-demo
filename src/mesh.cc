#include "mesh.h"
#include "obj_loader.h"
 
Mesh::Mesh(std::string file, GLuint shader)
{
	m_file = file;
	m_modelMatrix = glm::mat4(1);
	m_translateMatrix = glm::mat4(1);
	m_rotationMatrix = glm::mat4(1);
	m_shader = shader;
	load();
}
 
Mesh::~Mesh()
{
	clear();
}
 
void Mesh::clear()
{
	m_vertices.clear();
	m_uvs.clear();
	m_normals.clear();
 
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_vcb);
	glDeleteVertexArrays(1, &m_vao);
}
 
void Mesh::draw_mesh()
{
	m_modelMatrix = m_translateMatrix * m_rotationMatrix;
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &m_modelMatrix[0][0]);
 
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}
 
void Mesh::translate(glm::vec3 position)
{
	m_translateMatrix = glm::mat4(1);
	m_translateMatrix = glm::translate(m_translateMatrix, position);
}
 
void Mesh::load()
{
	load_obj(m_file.c_str(), m_vertices, m_uvs, m_normals);
 
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		m_vertices.size() * sizeof(glm::vec3),
		&m_vertices[0],
		GL_STATIC_DRAW);
 
	glGenBuffers(1, &m_vcb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vcb);
	glBufferData(
		GL_ARRAY_BUFFER,
		m_normals.size() * sizeof(glm::vec3),
		&m_normals[0],
		GL_STATIC_DRAW);
 
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);
 
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vcb);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);
 
	m_modelMatrixID = glGetUniformLocation(m_shader, "mm");
}
