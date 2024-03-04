#pragma once

#include "miscellaneous/singleton_instanciator.hpp"

#include "object/player_object.hpp"

class MainWidget : public spk::IWidget
{
private:
    spk::GameEngine _gameEngine;

    SingletonInstanciator _singletonInstanciator;
    
    spk::GameEngineManager _gameEngineManager;

    PlayerObject _playerObject;
    spk::Tilemap2D _tilemap2D;

    std::unique_ptr<spk::Notifier::Contract> _onChunkUpdateContract;

    void _onGeometryChange();

    void _loadTilemapNode();

public:
    MainWidget(const std::string& p_name);
    ~MainWidget();
};