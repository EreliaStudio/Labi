#pragma once

#include "sparkle.hpp"

#include "component/motion_controller.hpp"

class ActorObject : public spk::GameObject
{
private:
	spk::MeshRenderer* _bodyRenderer;
	MotionController* _motionController;

public:
	ActorObject(const std::string& p_name, spk::GameObject* p_parent = nullptr);

	bool isMoving() const;

	void requestMotion(const spk::Vector3& p_deltaPosition, const long long& p_mouvementDuration);

	void setSpriteSheet(const spk::SpriteSheet* p_spriteSheet);
	const spk::SpriteSheet* spriteSheet() const;

	void setSprite(const spk::Vector2UInt& p_spriteID);
	const spk::Vector2UInt& sprite() const;

	void setSpriteAnimation(const spk::SpriteAnimation* p_spriteAnimation);
	const spk::SpriteAnimation* spriteAnimation() const;
};