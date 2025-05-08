#pragma once
#include "BaseEditor.h"

class ECSEditor : public BaseEditor
{
public:
	void onAttach() override;
	void update() override;
	void fixedUpdate() override;
};