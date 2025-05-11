#include "Editors/ECSEditor.h"
#include "ECS/ECS.h"
#include "Game.h"
#include "GameTime.h"
#include "Log.h"
#include "Random.h"
#include "Rendering/Primitives.h"
#include "ECSGame/Scene.h"

#include "ECSGame/Components/Tag.h"
#include "ECSGame/Components/Transform.h"
#include "ECSGame/Components/Camera.h"
#include "ECSGame/Components/MeshRenderer.h"
#include "ECSGame/Components/Collider.h"
#include "ECSGame/Components/Rigidbody.h"
#include "ECSGame/Utils/PhysicsUtil.h"
#include "ECSGame/Utils/TransformUtil.h"
#include "ECSGame/Systems/FreeMovementSystem.h"

using namespace ECSGame;

static Scene* scene = nullptr;
static FreeMovementSystem* freeMovementSystem = nullptr;

static void initializeECSRegistry();

static void setUpTestScene();
static void createFloor();
static void createSpheres();
static void createBox();

void ECSEditor::onAttach()
{
	BaseEditor::onAttach();
	m_fpsCounter.start("ECSEditor");
	initializeECSRegistry();
	setUpTestScene();
}

void ECSEditor::update()
{
	BaseEditor::update();
	freeMovementSystem->update();
}

void ECSEditor::fixedUpdate()
{
	BaseEditor::fixedUpdate();
}

void initializeECSRegistry()
{
	//custom components registration
	ECS::registerComponentType<FreeMovement>();

	//registry initialization
	scene = Game::get().createScene<Scene>();

	//custom systems registration
	freeMovementSystem = scene->getRegistry().addSystem<FreeMovementSystem>();
}


void setUpTestScene()
{
	ECS::Entity cameraEntity = scene->getRegistry().createEntity();
	Transform cameraTransform = { {0, 30, 45} };
	scene->getRegistry().addComponent<Transform>(cameraEntity, cameraTransform);
	scene->getRegistry().addComponent<Camera>(cameraEntity, {});
	scene->getRegistry().addComponent<FreeMovement>(cameraEntity, { 5.0f, 25.0f, 0.04f });

	/*
	ECS::Entity cubeEntity = scene->getRegistry().createEntity();
	scene->getRegistry().addComponent<Transform>(cubeEntity, {});
	std::shared_ptr<Mesh> cubeMesh = Primitives::createCubeMesh();
	std::shared_ptr<Material> floorMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag",
		"../assets/textures/stratum.png");
	scene->getRegistry().addComponent<MeshRenderer>(cubeEntity, { cubeMesh, floorMat });
	*/

	createFloor();
	createSpheres();
	createBox();
}

void createFloor()
{
	ECS::Entity floorEntity = scene->getRegistry().createEntity();
	Transform floorTransform =
	{
		{ 0.0f, 0.0f, 0.0f }, //pos
		{ -90.0f, 0.0f, 0.0f }, //rot
		{ 100.0f, 100.0f, 0.1f } //scale
	};
	scene->getRegistry().addComponent<Transform>(floorEntity, floorTransform);
	std::shared_ptr<Mesh> quadMesh = Primitives::createQuadMesh();
	std::shared_ptr<Material> floorMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag");
	floorMat->setColor({ 0.6f, 0.3f, 0.2f, 1.0f });
	scene->getRegistry().addComponent<MeshRenderer>(floorEntity, { quadMesh, floorMat });

	scene->getRegistry().addComponent<Collider>(floorEntity, { Collider::Type::Box });
	PhysicsUtil::setBoxColliderHalfExtents(
		scene->getRegistry().getComponent<Collider>(floorEntity),
		floorTransform.m_scale * 0.5f);
	scene->getRegistry().addComponent<Rigidbody>(floorEntity, {});
	Rigidbody& floorRb = scene->getRegistry().getComponent<Rigidbody>(floorEntity);
	PhysicsUtil::setRigidbodyMass(floorRb, 0.0f); //make it static
	PhysicsUtil::setRigidbodyRestitution(floorRb, 0.75f);
}

void createSpheres()
{
	const size_t N_SPHERES = 2500;
	const glm::vec3 ORIGIN = { 0, 25, 0 };
	const float SPREAD = 10.0f;
	std::shared_ptr<Material> sphereMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag",
		"../assets/textures/laura.png");
	std::shared_ptr<Mesh> sphereMesh = Primitives::createSphereMesh();
	for (size_t i = 0; i < N_SPHERES; i++)
	{
		Transform sphereTransform;
		sphereTransform.m_position = ORIGIN + Random::pointInUnitSphere() * SPREAD;
		sphereTransform.m_position.y += 15;
		sphereTransform.m_rotation = { 0, 90, -90 };

		ECS::Entity sphereEntity = scene->getRegistry().createEntity();
		scene->getRegistry().addComponent<Transform>(sphereEntity, sphereTransform);
		scene->getRegistry().addComponent<MeshRenderer>(sphereEntity, { sphereMesh, sphereMat });

		scene->getRegistry().addComponent<Collider>(sphereEntity, { Collider::Type::Sphere });
		PhysicsUtil::setSphereColliderRadius(
			scene->getRegistry().getComponent<Collider>(sphereEntity),
			1.0f);
		scene->getRegistry().addComponent<Rigidbody>(sphereEntity, {});
		Rigidbody& rb = scene->getRegistry().getComponent<Rigidbody>(sphereEntity);
		PhysicsUtil::setRigidbodyMass(rb, 1.0f);
		PhysicsUtil::setRigidbodyRestitution(rb, 0.75f);
	}
}

void createBox()
{
	std::shared_ptr<Mesh> boxMesh = Primitives::createCubeMesh();
	std::shared_ptr<Material> boxMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag");
	boxMat->setColor({ 1, 1, 1, 0.3f });
	glm::vec3 boxRotations[] =
	{
		{ 180, 0,   0},
		{   0, 0,  90},
		{  90, 0,   0},
		{   0, 0, -90},
		{ -90, 0,   0}
	};
	for (size_t i = 0; i < 5; i++)
	{

		ECS::Entity boxEntity = scene->getRegistry().createEntity();
		Transform boxTransform;
		boxTransform.m_position = { 0.0f, 5.0f, 0.0f };
		boxTransform.m_rotation = boxRotations[i];
		boxTransform.m_scale = { 20.0f, 0.5f, 20.0f };
		if (i > 0)
		{
			boxTransform.m_position += glm::vec3{ 0, 10, 0 } + TransformUtil::getUp(boxTransform) * 10.0f;
		}

		scene->getRegistry().addComponent<Transform>(boxEntity, boxTransform);
		scene->getRegistry().addComponent<MeshRenderer>(boxEntity, { boxMesh, boxMat });

		scene->getRegistry().addComponent<Collider>(boxEntity, { Collider::Type::Box });
		PhysicsUtil::setBoxColliderHalfExtents(
			scene->getRegistry().getComponent<Collider>(boxEntity),
			boxTransform.m_scale * 0.5f);
		scene->getRegistry().addComponent<Rigidbody>(boxEntity, {});
		Rigidbody& boxRb = scene->getRegistry().getComponent<Rigidbody>(boxEntity);
		PhysicsUtil::setRigidbodyMass(boxRb, 0.0f); //static
		PhysicsUtil::setRigidbodyRestitution(boxRb, 0.75f);
	}
}

