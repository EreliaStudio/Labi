#pragma once

#include "sparkle.hpp"

class MotionController : public spk::GameComponent
{
private:
	bool _isMoving;
	spk::Vector3 _startingPosition;
	spk::Vector3 _destination;
	long long _startingTime;
	long long _duration;

	void _onRender();
	void _onUpdate();

public:
	MotionController(const std::string& p_name);

	bool isMoving() const;

	void requestMotion(const spk::Vector3& p_deltaPosition, const long long& p_mouvementDuration);
};