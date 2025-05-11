#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
	setupMesh(vertices, indices);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	if (m_ebo)
	{
		glDeleteBuffers(1, &m_ebo);
	}
}

//vertices in this format (for every vertex:) posx, posy, posz, colorR, colorG, colorB, uvX, uvY
void Mesh::setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
	m_vertexCount = vertices.size() / 8; // Assuming 8 floats per vertex
	m_indexCount = indices.size();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	if (!indices.empty())
	{
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	// configure the active vao
	//position attribute:
	GLuint positionAttributeLocation = 0u;  //the location specificated in the vertex shader
	glVertexAttribPointer(positionAttributeLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(positionAttributeLocation); //enable the attribute

	//color attribute:
	GLuint colorAttributeLocation = 1u;  //the location specificated in the vertex shader
	glVertexAttribPointer(colorAttributeLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(colorAttributeLocation); //enable the attribute


	//uv attribute:
	GLuint uvAttributeLocation = 2u;  //the location specificated in the vertex shader
	glVertexAttribPointer(uvAttributeLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(uvAttributeLocation); //enable the attribute


	glBindVertexArray(0);
}

void Mesh::bind() const
{
	glBindVertexArray(m_vao);
}

void Mesh::render() const
{

	if (m_indexCount > 0)
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}

void Mesh::unbind() const
{
	glBindVertexArray(0);
}

