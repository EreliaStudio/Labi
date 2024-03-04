#pragma once

#include "object/actor_object.hpp"

#include "component/player_controller.hpp"

class PlayerObject : public ActorObject
{
private:
    spk::Camera _cameraObject;
	PlayerController* _controller;

public:
	PlayerObject(const std::string& p_name, spk::GameObject* p_parent = nullptr);

	spk::Camera& camera();
};