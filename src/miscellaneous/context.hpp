#pragma once

#include "sparkle.hpp"

#include "object/player_object.hpp"

struct Context : public spk::Singleton<Context>
{
	const PlayerObject* playerObject;
	const spk::Tilemap2D* tilemap;
};