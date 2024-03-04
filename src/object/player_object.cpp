#include "object/player_object.hpp"
#include "miscellaneous/texture_atlas.hpp"

PlayerObject::PlayerObject(const std::string& p_name, spk::GameObject* p_parent) :
	ActorObject(p_name, p_parent),
	_cameraObject("Camera", this),
	_controller(addComponent<PlayerController>("Controller"))
{ 
	_cameraObject.transform().translation = spk::Vector3(0, 0, 5);
	_cameraObject.transform().lookAt(spk::Vector3(0, 0, 0));

	_cameraObject.setType(spk::Camera::Type::Orthographic);
	_cameraObject.setAsMainCamera();

	setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("PlayerSprite"));
}

spk::Camera& PlayerObject::camera()
{
	return (_cameraObject);
}