#pragma once
#include "BaseEditor.h"

class ECSEditor : public BaseEditor
{
public:
	virtual void onAttach() override;
	virtual void update() override;
	virtual void fixedUpdate() override;
};