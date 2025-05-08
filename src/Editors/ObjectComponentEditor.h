#pragma once
#include "BaseEditor.h"

class ObjectComponentEditor : public BaseEditor
{
public:
	void onAttach() override;
	void update() override;
	void fixedUpdate() override;
};
