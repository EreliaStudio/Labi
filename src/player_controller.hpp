#pragma once

#include "sparkle.hpp"

#include "event_source.hpp"

class PlayerController : public spk::IWidget
{
private:
    spk::GameObject _playerObject;
    spk::GameObject _cameraObject;
    spk::Camera* _cameraComponent;
    spk::MeshRenderer *_playerBodyRenderer;

    spk::Tilemap2D* _tilemap;

    long long _startingTime;
    long long _endingTime;
    spk::Vector3 _startingPosition;
    spk::Vector3 _endingPosition;
    spk::Vector2 _nbTileOnScreen;

    void _onGeometryChange();

    void _onUpdate();

public:
    PlayerController(const std::string& p_name, spk::IWidget* p_parent);

    void setTilemap(spk::Tilemap2D* p_tilemap);

    spk::GameObject& playerObject();
    spk::Camera* cameraComponent();
    spk::MeshRenderer* playerBodyRenderer();
};