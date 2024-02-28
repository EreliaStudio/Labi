#pragma once

#include "sparkle.hpp"
#include "singleton_instanciator.hpp"
#include "player_controller.hpp"
#include "tilemap_manager.hpp"

class MainWidget : public spk::IWidget
{
private:
    spk::GameEngine _gameEngine;

    SingletonInstanciator _singletonInstanciator;
    
    spk::GameEngineManager _gameEngineManager;
    PlayerController _playerController;
    TilemapManager _tilemapManager;

    void _onGeometryChange();

public:
    MainWidget(const std::string& p_name);
    ~MainWidget();
};