#include "Editor.h"
#include "Game.h"
#include "Input.h"
#include "Log.h"
#include "ObjectComponent/Camera.h"
#include "ObjectComponent/Transform.h"
#include "ObjectComponent/MeshRenderer.h"
#include "ObjectComponent/Primitives.h"
#include <glm/glm.hpp>

using namespace ObjectComponent;

void Editor::onAttach()
{
	setUpTestScene();
}

void Editor::update(float deltaTime)
{
	moveCamera(deltaTime);
	rotateCamera(deltaTime);
}

void Editor::fixedUpdate()
{
}



void Editor::setUpTestScene()
{
	Scene* scene = Game::get().createScene();
	GameObject* cam = scene->createGameObject("Camera");
	cam->addComponent<Camera>();
	cam->getTransform()->setPosition({ 0, 3, 3 });


	std::shared_ptr<Mesh> quadMesh = Primitives::createQuadMesh();
	std::shared_ptr<Material> floorMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag");
	floorMat->setColor({ 0.06f, 0.3f, 0.2f, 1.0f });
	GameObject* floor = scene->createGameObject("Floor");
	MeshRenderer* meshRenderer = floor->addComponent<MeshRenderer>();
	meshRenderer->setMesh(quadMesh);
	meshRenderer->setMaterial(floorMat);
	Transform* floorTransform = floor->getTransform();
	floorTransform->setRotation({ -90, 0, 0 });
	floorTransform->setScale({ 10, 10, 10 });


	std::shared_ptr<Material> sphereMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag",
		"../assets/textures/laura.png");
	std::shared_ptr<Mesh> sphereMesh = Primitives::createSphereMesh();
	std::vector<glm::vec3> positions =
	{
		{ 5, 3,  0},
		{-5, 3,  0},
		{ 0, 3,  5},
		{ 0, 3, -5}
	};
	for (size_t i = 0; i < 4; i++)
	{
		GameObject* sphere = scene->createGameObject("Sphere " + std::to_string(i));
		Transform* transform = sphere->getTransform();
		transform->setPosition(positions[i]);
		transform->setRotation({ 0, 90, -90 });
		MeshRenderer* meshRenderer = sphere->addComponent<MeshRenderer>();
		meshRenderer->setMesh(sphereMesh);
		meshRenderer->setMaterial(sphereMat);
	}
}

void Editor::moveCamera(float deltaTime)
{
	if (Game::get().getScene()->getActiveCamera() == nullptr)
		return;

	glm::vec3 moveInput = { 0, 0, 0 };
	float speed = cameraMoveParams.speed;

	if (Input::isKeyHeld(GLFW_KEY_W))
	{
		moveInput.y += 1;
	}
	if (Input::isKeyHeld(GLFW_KEY_S))
	{
		moveInput.y -= 1;
	}
	if (Input::isKeyHeld(GLFW_KEY_A))
	{
		moveInput.x -= 1;
	}
	if (Input::isKeyHeld(GLFW_KEY_D))
	{
		moveInput.x += 1;
	}
	if (Input::isKeyHeld(GLFW_KEY_SPACE))
	{
		moveInput.z += 1;
	}
	if (Input::isKeyHeld(GLFW_KEY_LEFT_CONTROL))
	{
		moveInput.z -= 1;
	}
	if (Input::isKeyHeld(GLFW_KEY_LEFT_SHIFT))
	{
		speed *= 4;
	}

	if (glm::length(moveInput) > 0.0f)
	{
		Transform* ct = Game::get().getScene()->getActiveCamera()->getGameObject()->getTransform();
		glm::vec3 movement = ct->getForward() * moveInput.y +
			ct->getRight() * moveInput.x +
			glm::vec3(0, 1, 0) * moveInput.z;

		ct->setPosition(ct->getPosition() + glm::normalize(movement) * speed * deltaTime);

		//LOG("Cam pos: " << ct->getPosition().x << ", " << ct->getPosition().y << ", " << ct->getPosition().z);
	}
}

void Editor::rotateCamera(float deltaTime)
{
	glm::vec2 offset = Input::getMouseDelta();
	if(glm::length(offset) != 0)
		LOG("deltamouse: " << offset.x << "  " << offset.y);
	offset *= cameraMoveParams.sensitivity * deltaTime;

	if (Camera* camera = Game::get().getScene()->getActiveCamera())
	{
		Transform* ct = camera->getGameObject()->getTransform();
		glm::vec3 rot = ct->getRotation();
		rot.y -= offset.x;
		rot.x = glm::clamp(rot.x + offset.y, -89.0f, 89.0f);
		ct->setRotation(rot);
	}
}
