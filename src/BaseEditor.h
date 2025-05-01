#pragma once

class BaseEditor
{
public:
	virtual void onAttach() = 0;
	virtual void update() = 0;
	virtual void fixedUpdate() = 0;
	virtual ~BaseEditor() = default;

};
