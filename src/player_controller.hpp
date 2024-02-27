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

    void _onGeometryChange();

    void _onUpdate();

public:
    PlayerController(const std::string& p_name, spk::IWidget* p_parent);

    spk::GameObject& playerObject();
    spk::Camera* cameraComponent();
    spk::MeshRenderer* playerBodyRenderer();
};