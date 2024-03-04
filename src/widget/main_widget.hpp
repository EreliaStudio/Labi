#pragma once

#include "miscellaneous/singleton_instanciator.hpp"

#include "miscellaneous/tilemap_flags.hpp"
#include "object/player_object.hpp"

class MainWidget : public spk::IWidget
{
private:
    spk::GameEngine _gameEngine;

    SingletonInstanciator _singletonInstanciator;
    
    spk::GameEngineManager _gameEngineManager;

    spk::Tilemap2D _tilemap2D;
    PlayerObject _playerObject;

    spk::RandomGenerator<int> _bushFightGenerator;

	spk::NotifierContract _notifierContracts[4];
    spk::NotifierContract _onChunkUpdateContract;

    void _onGeometryChange();

    void _loadTilemapNode();

    void _generateFightArea();
	void _movePlayer(const spk::Vector3& p_deltaPosition, const long long& p_duration);

    void _onUpdate();

public:
    MainWidget(const std::string& p_name);
    ~MainWidget();
};