#include "component/player_controller.hpp"

#include "miscellaneous/context.hpp"
#include "miscellaneous/event_source.hpp"

void PlayerController::_onRender()
{

}

void PlayerController::_onUpdate()
{
	if (_motionController->isMoving() == true)
		return ;

	static const spk::Keyboard::Key keys[4] = {
		spk::Keyboard::Z,
		spk::Keyboard::Q,
		spk::Keyboard::S,
		spk::Keyboard::D
	};
	static const Event events[4] = {
		Event::PlayerMotionUp,
		Event::PlayerMotionLeft,
		Event::PlayerMotionDown,
		Event::PlayerMotionRight
	};
	
	for (size_t i = 0; i < 4 && _motionController->isMoving() == false; i++)
	{
		if (spk::Application::activeApplication()->keyboard().getKey(keys[i]) == spk::InputState::Down)
		{
			EventSource::instance()->notify_all(events[i]);
		}
	}
}

void PlayerController::_movePlayer(const spk::Vector3& p_deltaPosition, const long long& p_duration)
{
	if (Context::instance()->tilemap->isObstacle(owner()->globalPosition() + p_deltaPosition) == false)
		_motionController->requestMotion(p_deltaPosition, p_duration);
}

PlayerController::PlayerController(const std::string& p_name) :
	spk::GameComponent(p_name),
	_motionController(owner()->getComponent<MotionController>()),
	_notifierContracts{
		EventSource::instance()->subscribe(Event::PlayerMotionUp,	 [&](){_movePlayer(spk::Vector3( 0,  1, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionLeft,  [&](){_movePlayer(spk::Vector3(-1,  0, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionDown,  [&](){_movePlayer(spk::Vector3( 0, -1, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionRight, [&](){_movePlayer(spk::Vector3( 1,  0, 0), 150);})
	}
{

}