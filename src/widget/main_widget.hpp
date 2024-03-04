#pragma once

#include "miscellaneous/singleton_instanciator.hpp"

#include "miscellaneous/tilemap_flags.hpp"
#include "object/player_object.hpp"
#include "object/battle_arena_object.hpp"

class MainWidget : public spk::IWidget
{
private:
    spk::GameEngine _gameEngine;

    SingletonInstanciator _singletonInstanciator;
    
    spk::GameEngineManager _gameEngineManager;

    spk::Tilemap2D _tilemap2D;
    PlayerObject _playerObject;

    BattleAreaObject _battleAreaObject;
    spk::RandomGenerator<int> _bushFightGenerator;

	spk::NotifierContract _playerMotionContracts[4];
    spk::NotifierContract _onChunkUpdateContract;
	spk::NotifierContract _enteringBattleModeContract;
	spk::NotifierContract _exitingBattleModeContract;

    void _onGeometryChange();

    void _loadTilemapNode();

    BattleAreaObject::TileObject::Type _convertNodeToBattleTile(const spk::Vector2Int& p_absolutePosition);
    void _populateBattleArea();
	void _movePlayer(const spk::Vector3& p_deltaPosition, const long long& p_duration);

    void _enterBattleMode();
    void _exitBattleMode();

    void _onUpdate();

public:
    MainWidget(const std::string& p_name);
    ~MainWidget();
};