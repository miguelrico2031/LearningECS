#pragma once

class Editor
{
public:
	void onAttach();
	void update(float deltaTime);
	void fixedUpdate();
private:
	void setUpTestScene();
	void moveCamera(float deltaTime);
	void rotateCamera(float deltaTime);
private:
	struct
	{
		float speed = 5.0f;
		float sensitivity = 5.0f;
	} cameraMoveParams;
};