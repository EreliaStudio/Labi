#pragma once

#include "sparkle.hpp"

class ActorObject : public spk::GameObject
{
private:
	spk::MeshRenderer* _bodyRenderer;

public:
	ActorObject(const std::string& p_name, spk::GameObject* p_parent = nullptr) :
		spk::GameObject(p_name, p_parent),
		_bodyRenderer(addComponent<spk::MeshRenderer>("BodyRenderer"))
	{

	}

	void setSpriteSheet(const spk::SpriteSheet* p_spriteSheet)
	{
		_bodyRenderer->setSpriteSheet(p_spriteSheet);
	}

	const spk::SpriteSheet* spriteSheet() const
	{
		return (_bodyRenderer->spriteSheet());
	}

	void setSprite(const spk::Vector2UInt& p_spriteID)
	{
		_bodyRenderer->setSprite(p_spriteID);
	}

	const spk::Vector2UInt& sprite() const
	{
		return (_bodyRenderer->sprite());
	}

	void setSpriteAnimation(const spk::SpriteAnimation* p_spriteAnimation)
	{
		_bodyRenderer->setSpriteAnimation(p_spriteAnimation);
	}
	
	const spk::SpriteAnimation* spriteAnimation() const
	{
		return (_bodyRenderer->spriteAnimation());
	}
};