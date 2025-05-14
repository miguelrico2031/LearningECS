#pragma once
#include "Mesh.h"
#include <memory>


namespace Primitives
{
	inline static std::shared_ptr<Mesh> createQuadMesh()
	{
		static const std::vector<float> vertices =
		{	 // positions			 // normals				// uvs
			 0.5f,  0.5f,  0.0f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
			 0.5f, -0.5f,  0.0f,	 0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
			-0.5f, -0.5f,  0.0f,	 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
			-0.5f,  0.5f,  0.0f,	 0.0f, 0.0f, 1.0f,		0.0f, 1.0f
		};


		static const std::vector<unsigned int> indices =
		{
			0, 3, 1,
			1, 3, 2
		};
		return std::make_shared<Mesh>(vertices, indices);
	}


	inline static std::shared_ptr<Mesh> createCubeMesh()
	{

		static const std::vector<float> vertices =
		{
			// Front face
			-0.5f, -0.5f,  0.5f,  0,  0,  1,  0, 0,
			 0.5f, -0.5f,  0.5f,  0,  0,  1,  1, 0,
			 0.5f,  0.5f,  0.5f,  0,  0,  1,  1, 1,
			-0.5f,  0.5f,  0.5f,  0,  0,  1,  0, 1,

			// Back face
			 0.5f, -0.5f, -0.5f,  0,  0, -1,  0, 0,
			-0.5f, -0.5f, -0.5f,  0,  0, -1,  1, 0,
			-0.5f,  0.5f, -0.5f,  0,  0, -1,  1, 1,
			 0.5f,  0.5f, -0.5f,  0,  0, -1,  0, 1,

			 // Left face
			 -0.5f, -0.5f, -0.5f, -1,  0,  0,  0, 0,
			 -0.5f, -0.5f,  0.5f, -1,  0,  0,  1, 0,
			 -0.5f,  0.5f,  0.5f, -1,  0,  0,  1, 1,
			 -0.5f,  0.5f, -0.5f, -1,  0,  0,  0, 1,

			 // Right face
			  0.5f, -0.5f,  0.5f,  1,  0,  0,  0, 0,
			  0.5f, -0.5f, -0.5f,  1,  0,  0,  1, 0,
			  0.5f,  0.5f, -0.5f,  1,  0,  0,  1, 1,
			  0.5f,  0.5f,  0.5f,  1,  0,  0,  0, 1,

			  // Top face
			  -0.5f,  0.5f,  0.5f,  0,  1,  0,  0, 0,
			   0.5f,  0.5f,  0.5f,  0,  1,  0,  1, 0,
			   0.5f,  0.5f, -0.5f,  0,  1,  0,  1, 1,
			  -0.5f,  0.5f, -0.5f,  0,  1,  0,  0, 1,

			  // Bottom face
			  -0.5f, -0.5f, -0.5f,  0, -1,  0,  0, 0,
			   0.5f, -0.5f, -0.5f,  0, -1,  0,  1, 0,
			   0.5f, -0.5f,  0.5f,  0, -1,  0,  1, 1,
			  -0.5f, -0.5f,  0.5f,  0, -1,  0,  0, 1,

		};

		static const std::vector<unsigned int> indices =
		{
			0,  1,  2,  2,  3,  0,      // Front
			4,  5,  6,  6,  7,  4,      // Back
			8,  9, 10, 10, 11,  8,      // Left
			12, 13, 14, 14, 15, 12,      // Right
			16, 17, 18, 18, 19, 16,      // Top
			20, 21, 22, 22, 23, 20       // Bottom
		};

		return std::make_shared<Mesh>(vertices, indices);
	}

	inline static std::shared_ptr<Mesh> createSphereMesh(unsigned int sectorCount = 36, unsigned int stackCount = 18)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		const float PI = 3.14159265359f;

		// Generate vertices
		for (unsigned int i = 0; i <= stackCount; ++i)
		{
			float stackAngle = PI / 2 - i * PI / stackCount; // from pi/2 to -pi/2
			float xy = cosf(stackAngle); // r * cos(u)
			float z = sinf(stackAngle);  // r * sin(u)

			for (unsigned int j = 0; j <= sectorCount; ++j)
			{
				float sectorAngle = j * 2 * PI / sectorCount; // from 0 to 2pi

				// Vertex position
				float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
				float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				// Vertex normal
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				// UV coordinates
				vertices.push_back((float)j / sectorCount);
				vertices.push_back((float)i / stackCount);
			}
		}

		// Generate indices
		for (unsigned int i = 0; i < stackCount; ++i)
		{
			for (unsigned int j = 0; j < sectorCount; ++j)
			{
				unsigned int first = (i * (sectorCount + 1)) + j;
				unsigned int second = first + sectorCount + 1;

				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}

		return std::make_shared<Mesh>(vertices, indices);
	}
}
