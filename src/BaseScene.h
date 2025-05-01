#pragma once

class BaseScene
{
public:
	BaseScene() = default;

	virtual void load() = 0;
	virtual void unload() = 0;
	virtual void update() = 0;
	virtual void fixedUpdate() = 0;
	virtual void render() = 0;
	virtual void onWindowResize(int width, int height) = 0;

	virtual ~BaseScene() = default;
};