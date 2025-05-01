#pragma once
#include <random>
#include <glm/glm.hpp>
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif 
#include <glm/gtx/norm.hpp>

namespace Random
{
    inline glm::vec3 pointInUnitSphere()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        glm::vec3 point;
        do
        {
            point = glm::vec3(dist(gen), dist(gen), dist(gen));
        } while (glm::length2(point) > 1.0f);

        return point;
    }

    inline float floatInRange(float min, float max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    inline int intInRange(int min, int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }
}