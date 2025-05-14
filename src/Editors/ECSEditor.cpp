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
#include <filesystem>

using namespace ECSGame;



void ECSEditor::onAttach(Game& game)
{
	m_game = &game;
	BaseEditor::onAttach(game);
	m_fpsCounter.start("ECSEditor", "analysis/");
	initializeECSRegistry();
	setUpTestScene();
}

void ECSEditor::update()
{
	BaseEditor::update();
	m_freeMovementSystem->update();
}

void ECSEditor::fixedUpdate()
{
	BaseEditor::fixedUpdate();
}

void ECSEditor::initializeECSRegistry()
{
	//custom components registration
	ECS::registerComponentType<FreeMovement>();

	//registry initialization
	m_scene = m_game->createScene<Scene>();

	//custom systems registration
	m_freeMovementSystem = m_scene->getRegistry().addSystem<FreeMovementSystem>();
}


void ECSEditor::setUpTestScene()
{
	ECS::Registry& reg = m_scene->getRegistry();
	ECS::Entity cameraEntity = reg.createEntity();
	Transform cameraTransform = { {0, 30, 45} };
	reg.addComponent<Transform>(cameraEntity, cameraTransform);
	reg.addComponent<Camera>(cameraEntity, {});
	reg.addComponent<FreeMovement>(cameraEntity, 
		{ m_params.m_cameraMoveSpeed, m_params.m_cameraSprintSpeed, m_params.m_cameraSensitivity });

	createFloor();
	createSpheres();
	createBox();
}

void ECSEditor::createFloor()
{
	ECS::Registry& reg = m_scene->getRegistry();
	ECS::Entity floorEntity = reg.createEntity();
	Transform floorTransform =
	{
		{ 0.0f, 0.0f, 0.0f }, //pos
		{ -90.0f, 0.0f, 0.0f }, //rot
		{ 100.0f, 100.0f, 0.1f } //scale
	};
	reg.addComponent<Transform>(floorEntity, floorTransform);
	std::shared_ptr<Mesh> quadMesh = Primitives::createQuadMesh();
	std::shared_ptr<Material> floorMat = std::make_shared<Material>(
		"assets/shaders/basic.vert",
		"assets/shaders/basic.frag",
		"assets/textures/wood.jpg");
	floorMat->setColor({ 0.6f, 0.3f, 0.2f, 1.0f });
	floorMat->setTiling(5);
	reg.addComponent<MeshRenderer>(floorEntity, { quadMesh, floorMat });

	reg.addComponent<Collider>(floorEntity, { Collider::Type::Box });
	PhysicsUtil::setBoxColliderHalfExtents(
		reg.getComponent<Collider>(floorEntity),
		floorTransform.m_scale * 0.5f);
	reg.addComponent<Rigidbody>(floorEntity, {});
	Rigidbody& floorRb = reg.getComponent<Rigidbody>(floorEntity);
	PhysicsUtil::setRigidbodyMass(floorRb, 0.0f); //make it static
	PhysicsUtil::setRigidbodyRestitution(floorRb, 0.75f);
}

void ECSEditor::createSpheres()
{
	ECS::Registry& reg = m_scene->getRegistry();

	//const size_t N_SPHERES = 2500;
	//const glm::vec3 ORIGIN = { 0, 40, 0 };
	//const float SPREAD = 10.0f;

	std::shared_ptr<Material> sphereMat = std::make_shared<Material>(
		"assets/shaders/basic.vert",
		"assets/shaders/basic.frag",
		"assets/textures/metal.jpg");
	sphereMat->setTiling(2);
	std::shared_ptr<Mesh> sphereMesh = Primitives::createSphereMesh();

	glm::vec3 origin = { m_params.m_spheresOrigin[0], m_params.m_spheresOrigin[1], m_params.m_spheresOrigin[2] };

	for (size_t i = 0; i < m_params.m_sphereCount; i++)
	{
		Transform sphereTransform;
		sphereTransform.m_position = origin + Random::pointInUnitSphere() * m_params.m_spheresSpread;
		sphereTransform.m_rotation = { 0, 90, -90 };

		ECS::Entity sphereEntity = reg.createEntity();
		reg.addComponent<Transform>(sphereEntity, sphereTransform);
		reg.addComponent<MeshRenderer>(sphereEntity, { sphereMesh, sphereMat });

		reg.addComponent<Collider>(sphereEntity, { Collider::Type::Sphere });
		PhysicsUtil::setSphereColliderRadius(
			reg.getComponent<Collider>(sphereEntity),
			1.0f);
		reg.addComponent<Rigidbody>(sphereEntity, {});
		Rigidbody& rb = reg.getComponent<Rigidbody>(sphereEntity);
		PhysicsUtil::setRigidbodyMass(rb, 1.0f);
		PhysicsUtil::setRigidbodyRestitution(rb, 0.75f);
	}
}

void ECSEditor::createBox()
{
	ECS::Registry& reg = m_scene->getRegistry();

	std::shared_ptr<Mesh> boxMesh = Primitives::createCubeMesh();
	std::shared_ptr<Material> boxMat = std::make_shared<Material>(
		"assets/shaders/basic.vert",
		"assets/shaders/basic.frag");
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

		ECS::Entity boxEntity = reg.createEntity();
		Transform boxTransform;
		boxTransform.m_position = { 0.0f, 5.0f, 0.0f };
		boxTransform.m_rotation = boxRotations[i];
		boxTransform.m_scale = { 20.0f, 0.5f, 20.0f };
		if (i > 0)
		{
			boxTransform.m_position += glm::vec3{ 0, 10, 0 } + TransformUtil::getUp(boxTransform) * 10.0f;
		}

		reg.addComponent<Transform>(boxEntity, boxTransform);
		reg.addComponent<MeshRenderer>(boxEntity, { boxMesh, boxMat });

		reg.addComponent<Collider>(boxEntity, { Collider::Type::Box });
		PhysicsUtil::setBoxColliderHalfExtents(
			reg.getComponent<Collider>(boxEntity),
			boxTransform.m_scale * 0.5f);
		reg.addComponent<Rigidbody>(boxEntity, {});
		Rigidbody& boxRb = reg.getComponent<Rigidbody>(boxEntity);
		PhysicsUtil::setRigidbodyMass(boxRb, 0.0f); //static
		PhysicsUtil::setRigidbodyRestitution(boxRb, 0.75f);
	}
}

