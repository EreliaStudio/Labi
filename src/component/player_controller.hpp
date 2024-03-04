#pragma once

#include "sparkle.hpp"

#include "component/motion_controller.hpp"

class PlayerController : public spk::GameComponent
{
private:
	MotionController* _motionController;

	spk::NotifierContract _notifierContracts[4];

	void _onRender();
	void _onUpdate();

	void _movePlayer(const spk::Vector3& p_deltaPosition, const long long& p_duration);

public:
	PlayerController(const std::string& p_name);
};