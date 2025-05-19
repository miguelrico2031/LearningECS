# Learning ECS
LearningECS is a C++ OpenGL project designed to demonstrate and compare two different architectural patterns in game development: Object-Component architecture and Entity-Component-System (ECS) architecture. The repository serves as both a learning tool and a (very basic) reference implementation for these two approaches.

LearningECS was created to explore and compare the performance characteristics and implementation differences between traditional Object-Component architecture and the more data-oriented Entity-Component-System architecture. The project implements the same functionality (a physics-based simulation) using both paradigms, allowing for direct comparison of their performance, memory usage, and code organization.

## Prerequisites
Ensure you have the following installed:

- [CMake](https://cmake.org/download/) (for building the project).
- [Visual Studio](https://visualstudio.microsoft.com/es/downloads/) (or any compatible IDE that supports CMake projects).
- A C++20 compatible compiler.

## Compilation Instructions (Visual Studio - MSVC)
1. Clone the repository:

        git clone https://github.com/miguelrico2031/LearningECS.git
        cd LearningECS
2. Create a build directory and run CMake to generate the Visual Studio project files:

        mkdir build
        cd build
        cmake ..
3. Open the generated `LearningECS.sln` file in Visual Studio.

4. Build the project within Visual Studio by pressing `Ctrl+Shift+B` or selecting "Build Solution" from the Build menu.

5. Run `build/bin/[Debug or Release]/LearningECS.exe`.

## Dependencies
- [**OpenGL**](https://www.khronos.org/api/index_2017/opengl): Used for rendering 3D objects.
- [**GLAD**](https://github.com/Dav1dde/glad): Used for loading OpenGL functions.
- [**GLFW**](https://github.com/glfw/glfw): Used for creating an OpenGL window and context, as well as event polling.
- [**GLM**](https://github.com/g-truc/glm): Used for vector and matrix math.
- [**Bullet Physics**](https://github.com/bulletphysics/bullet3): The physics engine used for both Object-Component and ECS rigid body and collider components.


## Structure
The repository implements a small game engine framework with both architectural patterns. The main components include:

- Core game engine with rendering and physics
- Object-Component implementation
- Entity-Component-System implementation
- Test simulation run identically in both architectures

## Performance Testing
The [`main.cpp`](https://github.com/miguelrico2031/LearningECS/blob/main/src/main.cpp) file does performance testing through repeated runs of both architectural implementations with identical parameters, by running a loop that runs each simulation a number of times with the same parameters, allowing for the collection of average FPS values across multiple runs. These values are then used to compare the overall performance and efficiency of both implementations, helping to determine which architecture provides better simulation speed and memory access patterns.


## Results
![Demo](readme_media/demo.gif/)
The [`analysis.py`](https://github.com/miguelrico2031/LearningECS/blob/main/analysis/analysis.py) file reads the performance test results, stored in JSON files, and compares them to identify the percentage improvement in average FPS of ECS over Object-Component.

After running the simulations 100 times, the results showed that the ECS implementation had an average FPS 9.5% higher than the Object-Component implementation. This improvement is negligible, and is mainly due to the fact that the physics components in ECS are merely wrappers around Bullet objects pointers (`btRigidBody` and `btCollisionShape`), meaning that memory indirection still occurs.

Nevertheless, the primary goal of this project was to demonstrate that a basic ECS implementation could improve performance over a traditional Object-Component architecture in contexts requiring the processing of many similar objects, thanks to the data-oriented design and the separation of data from logic. Therefore, it has been a success.

### Strengths
#### Data-Oriented Design
The project demonstrates the benefits of ECS’s data-oriented design, which can improve performance in scenarios where many similar entities need to be processed.

#### Modular Architecture
The system is highly modular, allowing for easy experimentation with components and systems, making it flexible for different use cases.

#### Easy to use API
The `ECSEditor` serves as an example of how to use the ECS API. It is very easy to add custom component types and systems, create entities and assign components to them, and even create wrapper classes around entities for a more OOP-style approach.

#### Learning Material
This project implements both Object-Component and ECS architectures, then builds a sample scene using both approaches. This is ideal for those who have never seen or used ECS but are familiar with classic OOP Object-Component architectures (such as Unity, Unreal, or Godot), as it provides an easy way to compare the two and understand how ECS differs in structure and design.

### Areas for improvement
#### Physics Performance
The performance improvement over the Object-Component architecture is minimal, primarily because the ECS physics components are wrappers for pointers to Bullet objects, which still result in memory indirection. This design choice was made because Bullet objects are not trivially copyable, which is a requirement for every component in the ECS implementation. A potential solution could involve managing Bullet objects separately, storing them contiguously in a dedicated manager, or modifying the ECS to use move semantics instead of copying components.

#### Memory Overhead
For each component type, an array is heap-allocated with space for one component per entity (up to a maximum number of entities). For example, if a camera component has a size of 32 bytes and the maximum entity count is set to 4096, the camera component array will allocate 131072 bytes. However, if only one camera entity exists, 131040 bytes are wasted. This issue arises for components that are rarely used. A potential solution could be to apply ECS only for components that most entities will have, while managing other components outside the ECS. Alternatively, [sparse sets](https://www.geeksforgeeks.org/sparse-set/) could be used to optimize memory usage.

#### Destroying entities
In the current ECS implementation, entities are destroyed by calling removeComponent for each component of the entity. This approach is costly because each system must then reorganize its subset of sorted entities after the removal. This introduces a significant performance overhead when working with a large number of entities. One possible improvement could involve using a more efficient way of marking entities as "inactive" or "destroyed" without needing to remove components individually. For instance, instead of iterating through each system and updating its entities after component removal, entities could be "soft-deleted" by marking them for future removal or by maintaining an "inactive" flag and only performing cleanup during a batch operation.

#### Disabling/Enabling components
Currently, the ECS lacks support for enabling and disabling components. To deactivate a component, it must be removed, and to reactivate it, it must be re-added. This approach is inefficient because it requires both the removal and re-adding of components, which also causes the loss of any component state. A more efficient approach could involve implementing a toggle system that allows components to be deactivated or reactivated without needing to remove and add them back, adding an "enabled" flag to each component. Alternatively, if a [sparse set](https://www.geeksforgeeks.org/sparse-set/) is implemented to store the components, the components wouldn't need to be sorted in the dense array (as the sparse array is the one that requires sorting). Instead, all active components could be placed at the beginning of the dense array, while all disabled components could be moved to the end.

#### Accessing Components
Currently, systems must manually call `getComponent` to access components, which can be inefficient and repetitive, especially when a system needs to access multiple components at once. This could be improved by using component views or grouped component access, similar to what is done in the [EnTT library](https://github.com/skypjack/entt). By implementing a component view system, each system could automatically access a subset of components that it requires, organized by the entity.