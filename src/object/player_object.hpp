#pragma once

#include "object/actor_object.hpp"

class PlayerObject : public ActorObject
{
private:
    spk::Camera _cameraObject;

public:
	PlayerObject(const std::string& p_name, spk::GameObject* p_parent = nullptr) :
		ActorObject(p_name, p_parent),
		_cameraObject("Camera", this)
	{ 
		_cameraObject.transform().translation = spk::Vector3(0, 0, 5);
		_cameraObject.transform().lookAt(spk::Vector3(0, 0, 0));

		_cameraObject.setType(spk::Camera::Type::Orthographic);
		_cameraObject.setAsMainCamera();

		setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("PlayerSprite"));
	}

	spk::Camera& camera() {return (_cameraObject); }
};