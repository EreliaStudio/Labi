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

PlayerController::PlayerController(const std::string& p_name) :
	spk::GameComponent(p_name),
	_motionController(owner()->getComponent<MotionController>())
{

}