#pragma once

#include "sparkle.hpp"

#include "object/actor_object.hpp"

#include "component/player_controller.hpp"

class PlayerObject : public ActorObject
{
public:
	enum class Mode
	{
		World,
		Battle
	};
private:
	spk::StateMachine<Mode> _stateMachine;
    spk::Camera _cameraObject;
	PlayerController* _controller;

public:
	PlayerObject(const std::string& p_name, spk::GameObject* p_parent = nullptr);

	void setMode(const Mode& p_mode);

	spk::Camera& camera();
};