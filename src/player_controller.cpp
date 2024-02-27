#include "player_controller.hpp"

#include "texture_atlas.hpp"

void PlayerController::_onGeometryChange()
{
	_cameraComponent->setOrthographicSize(size() / 64);
	EventSource::instance()->notify_all(Event::UpdateVisibleChunk);
}

void PlayerController::_onUpdate()
{
	static long long nextInput = spk::getTime();
	long long currentNextInput = nextInput;
	long long time = spk::getTime();

	spk::Keyboard::Key keys[4] = {
		spk::Keyboard::Z,
		spk::Keyboard::S,
		spk::Keyboard::Q,
		spk::Keyboard::D};

	spk::Vector3 deltas[4] = {
		spk::Vector3(0, 0.1f, 0),
		spk::Vector3(0, -0.1f, 0),
		spk::Vector3(-0.1f, 0, 0),
		spk::Vector3(0.1f, 0, 0)};

	spk::Vector3 nextDelta = 0;

	for (size_t i = 0; i < 4; i++)
	{
		if (spk::Application::activeApplication()->keyboard().getKey(keys[i]) == spk::InputState::Down &&
			currentNextInput <= spk::getTime())
		{
			nextDelta += deltas[i];
			nextInput = time + 10;
		} 
	}

	if (nextDelta != 0)
	{
		_playerObject.transform().translation += nextDelta; 
		EventSource::instance()->notify_all(Event::UpdateVisibleChunk);
	} 
}

PlayerController::PlayerController(const std::string &p_name, spk::IWidget *p_parent) :
	spk::IWidget(p_name, p_parent),
	_playerObject("Player"),
	_cameraObject("Camera", &_playerObject),
	_cameraComponent(_cameraObject.addComponent<spk::Camera>("Camera")),
	_playerBodyRenderer(_playerObject.addComponent<spk::MeshRenderer>("BodyRenderer"))
{
	_playerObject.transform().translation = spk::Vector3(0, 0, 2.5f);
	_playerBodyRenderer->setSprite(spk::Vector2Int(0, 0));

	_cameraObject.transform().translation = spk::Vector3(0, 0, 5);
	_cameraObject.transform().lookAt(_playerObject.transform().translation.get());

	_cameraComponent->setAsMainCamera();
	_cameraComponent->setType(spk::Camera::Type::Orthographic);
	_cameraComponent->setNearPlane(0);
	_cameraComponent->setFarPlane(8);
	_cameraComponent->setOrthographicSize(10);
	_cameraComponent->setAspectRatio(1.0f);
	_cameraComponent->setFOV(75);

	_playerBodyRenderer->setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("PlayerSprite"));

}

spk::GameObject &PlayerController::playerObject()
{
	return (_playerObject);
}

spk::Camera *PlayerController::cameraComponent()
{
	return (_cameraComponent);
}

spk::MeshRenderer *PlayerController::playerBodyRenderer()
{
	return (_playerBodyRenderer);
}