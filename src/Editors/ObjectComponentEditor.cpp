#include "Editors/ObjectComponentEditor.h"
#include "Game.h"
#include "Input.h"
#include "Log.h"
#include "GameTime.h"
#include "Random.h"
#include "ObjectComponent/Scene.h"
#include "ObjectComponent/Camera.h"
#include "ObjectComponent/Transform.h"
#include "ObjectComponent/MeshRenderer.h"
#include "ObjectComponent/Primitives.h"
#include "ObjectComponent/Physics.h"
#include "ObjectComponent/Rigidbody.h"
#include "ObjectComponent/Collider.h"

#include <glm/glm.hpp>

using namespace ObjectComponent;

static void setUpTestScene();
static void createFloor();
static void createSpheres();
static void createBox();

static void moveCamera();
static void rotateCamera();


static struct
{
	float speed = 5.0f;
	float sensitivity = 0.05f;
} cameraMoveParams;

static Scene* scene = nullptr;


void ObjectComponentEditor::onAttach()
{
	setUpTestScene();
}


void ObjectComponentEditor::update()
{
	static float counter = 0.0f;
	static int frames = 0;
	counter += GameTime::getDeltaTime(true);
	frames++;
	if (counter >= 1.0f)
	{
		LOG("FPS: " << frames);
		counter = 0.0f;
		frames = 0;
	}

	moveCamera();
	rotateCamera();
}

void ObjectComponentEditor::fixedUpdate() {}



void setUpTestScene()
{
	scene = Game::get().createScene<Scene>();

	//camera
	GameObject* cam = scene->createGameObject("Camera");
	Camera* camera = cam->addComponent<Camera>();
	cam->getTransform()->setPosition({ 0, 30, 30 });

	createFloor();
	createSpheres();
	createBox();
}

void createFloor()
{
	GameObject* floor = scene->createGameObject("Floor");

	Transform* floorTransform = floor->getTransform();
	floorTransform->setRotation({ -90, 0, 0 });
	floorTransform->setScale({ 100.0f, 100.0f, 0.1f });

	std::shared_ptr<Mesh> quadMesh = Primitives::createQuadMesh();
	std::shared_ptr<Material> floorMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag");
	floorMat->setColor({ 0.06f, 0.3f, 0.2f, 1.0f });
	MeshRenderer* meshRenderer = floor->addComponent<MeshRenderer>();
	meshRenderer->setMesh(quadMesh);
	meshRenderer->setMaterial(floorMat);

	BoxCollider* floorCol = floor->addComponent<BoxCollider>();
	floorCol->setHalfExtents(floorTransform->getScale() * 0.5f);
	Rigidbody* floorRb = floor->addComponent<Rigidbody>();
	//make it static
	floorRb->getBulletRigidbody()->setMassProps(0.0f, btVector3(0, 0, 0));
	floorRb->getBulletRigidbody()->setRestitution(0.75f);
}



void createSpheres()
{
	const size_t N_SPHERES = 600;
	const glm::vec3 ORIGIN = { 0, 25, 0 };
	const float SPREAD = 10.0f;

	std::shared_ptr<Material> sphereMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag",
		"../assets/textures/laura.png");
	std::shared_ptr<Mesh> sphereMesh = Primitives::createSphereMesh();

	for (size_t i = 0; i < N_SPHERES; i++)
	{

		glm::vec3 position = ORIGIN + Random::pointInUnitSphere() * SPREAD;
		position.y += 15;

		GameObject* sphere = scene->createGameObject("Sphere " + std::to_string(i));
		Transform* transform = sphere->getTransform();
		transform->setPosition(position);
		//transform->setRotation({ 0, 90, -90 });
		MeshRenderer* meshRenderer = sphere->addComponent<MeshRenderer>();
		meshRenderer->setMesh(sphereMesh);
		meshRenderer->setMaterial(sphereMat);

		SphereCollider* sphereCol = sphere->addComponent<SphereCollider>();
		sphereCol->setRadius(1.0f);
		Rigidbody* rb = sphere->addComponent<Rigidbody>();
		rb->getBulletRigidbody()->setRestitution(0.75f);
	}
}



void createBox()
{
	std::shared_ptr<Material> boxMat = std::make_shared<Material>(
		"../assets/shaders/basic.vert",
		"../assets/shaders/basic.frag");
	boxMat->setColor({ 1, 1, 1, 0.3f });

	std::shared_ptr<Mesh> boxMesh = Primitives::createCubeMesh();

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

		GameObject* box = scene->createGameObject("Box" + std::to_string(i));
		Transform* boxTransform = box->getTransform();
		boxTransform->setScale({ 20, 0.5f, 20 });
		boxTransform->setPosition({ 0, 5, 0 });
		boxTransform->setRotation(boxRotations[i]);

		if (i > 0)
		{
			boxTransform->translate({ 0, 10, 0 });
			glm::vec3 up = boxTransform->getUp();
			boxTransform->translate(up * 10.0f);
		}

		MeshRenderer* boxMeshRenderer = box->addComponent<MeshRenderer>();
		boxMeshRenderer->setMesh(boxMesh);
		boxMeshRenderer->setMaterial(boxMat);
		BoxCollider* boxCol = box->addComponent<BoxCollider>();
		boxCol->setHalfExtents(boxTransform->getScale() * 0.5f);
		Rigidbody* boxRb = box->addComponent<Rigidbody>();
		boxRb->getBulletRigidbody()->setMassProps(0.0f, btVector3(0, 0, 0));
		boxRb->getBulletRigidbody()->setRestitution(0.75f);
	}
}
#pragma region CAMERA
void moveCamera()
{
	if (!scene || !scene->getActiveCamera())
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
		Transform* ct = scene->getActiveCamera()->getGameObject()->getTransform();
		glm::vec3 movement = ct->getForward() * moveInput.y +
			ct->getRight() * moveInput.x +
			glm::vec3(0, 1, 0) * moveInput.z;

		ct->setPosition(ct->getPosition() + glm::normalize(movement) * speed * GameTime::getDeltaTime());

		//LOG("Cam pos: " << ct->getPosition().x << ", " << ct->getPosition().y << ", " << ct->getPosition().z);
	}
}

void rotateCamera()
{
	if (!scene || !scene->getActiveCamera())
		return;
	glm::vec2 offset = Input::getMouseDelta();
	offset *= cameraMoveParams.sensitivity * GameTime::getDeltaTime();
	Camera* camera = scene->getActiveCamera();
	Transform* ct = camera->getGameObject()->getTransform();
	glm::vec3 rot = ct->getRotation();
	rot.y -= offset.x;
	rot.x = glm::clamp(rot.x + offset.y, -89.0f, 89.0f);
	ct->setRotation(rot);
}
#pragma endregion
