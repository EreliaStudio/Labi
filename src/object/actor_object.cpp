#include "object/actor_object.hpp"

ActorObject::ActorObject(const std::string& p_name, spk::GameObject* p_parent) :
	spk::GameObject(p_name, p_parent),
	_bodyRenderer(addComponent<spk::MeshRenderer>("BodyRenderer")),
	_motionController(addComponent<MotionController>("MotionController"))
{

}

bool ActorObject::isMoving() const
{
	return (_motionController->isMoving());
}

void ActorObject::requestMotion(const spk::Vector3& p_deltaPosition, const long long& p_mouvementDuration)
{
	_motionController->requestMotion(p_deltaPosition, p_mouvementDuration);
}

void ActorObject::setSpriteSheet(const spk::SpriteSheet* p_spriteSheet)
{
	_bodyRenderer->setSpriteSheet(p_spriteSheet);
}

const spk::SpriteSheet* ActorObject::spriteSheet() const
{
	return (_bodyRenderer->spriteSheet());
}

void ActorObject::setSprite(const spk::Vector2UInt& p_spriteID)
{
	_bodyRenderer->setSprite(p_spriteID);
}

const spk::Vector2UInt& ActorObject::sprite() const
{
	return (_bodyRenderer->sprite());
}

void ActorObject::setSpriteAnimation(const spk::SpriteAnimation* p_spriteAnimation)
{
	_bodyRenderer->setSpriteAnimation(p_spriteAnimation);
}

const spk::SpriteAnimation* ActorObject::spriteAnimation() const
{
	return (_bodyRenderer->spriteAnimation());
}