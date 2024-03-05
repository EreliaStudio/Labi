#pragma once

#include "sparkle.hpp"

#include "object/player_object.hpp"
#include "object/battle_arena_object.hpp"

struct Context : public spk::Singleton<Context>
{
    spk::GameEngine gameEngine;
	PlayerObject playerObject = PlayerObject("Player");
	spk::Tilemap2D tilemap = spk::Tilemap2D("Tilemap");
    BattleArenaObject battleAreaObject = BattleArenaObject("BattleArea");
};