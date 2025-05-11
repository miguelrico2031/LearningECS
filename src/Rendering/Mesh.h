#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


class Mesh
{
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {});
	~Mesh();

	void bind() const;
	void render() const;
	void unbind() const;

private:
	size_t m_vertexCount = 0;
	size_t m_indexCount = 0;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

	void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
};
