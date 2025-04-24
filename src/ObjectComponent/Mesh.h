#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace ObjectComponent {
    class Mesh {
    public:
        Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {});
        ~Mesh();

        void render() const;

    private:
        GLuint m_vao = 0;
        GLuint m_vbo = 0;
        GLuint m_ebo = 0;
        size_t m_vertexCount = 0;
        size_t m_indexCount = 0;

        void setupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    };
}
