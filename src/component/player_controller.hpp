#pragma once

#include "sparkle.hpp"

#include "component/motion_controller.hpp"

class PlayerController : public spk::GameComponent
{
private:
	MotionController* _motionController;

	void _onRender();
	void _onUpdate();

public:
	PlayerController(const std::string& p_name);
};