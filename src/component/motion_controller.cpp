#include "component/motion_controller.hpp"

void MotionController::_onRender()
{

}

void MotionController::_onUpdate()
{
	if (isMoving() == false)
		return ;

	if (spk::Application::activeApplication()->timeManager().deltaTime() == 0)
		return ;

	long long currentTime = spk::Application::activeApplication()->timeManager().time() - _startingTime;
	float timeRatio = (static_cast<float>(currentTime) / static_cast<float>(_duration));
	owner()->transform().translation = spk::Vector3::lerp(_startingPosition, _destination, timeRatio);

	if (currentTime >= _duration)
		_isMoving = false;
}

MotionController::MotionController(const std::string& p_name) :
	spk::GameComponent(p_name),
	_isMoving(false)
{

}

bool MotionController::isMoving() const
{
	return (_isMoving);
}

void MotionController::requestMotion(const spk::Vector3& p_deltaPosition, const long long& p_mouvementDuration)
{
	_startingPosition = owner()->transform().translation.get();
	_destination = _startingPosition + p_deltaPosition;
	_startingTime = spk::Application::activeApplication()->timeManager().time();
	_duration = p_mouvementDuration;
	_isMoving = true;
}