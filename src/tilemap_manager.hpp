#pragma once

#include "sparkle.hpp"

#include "event_source.hpp"
#include "tilemap_generator.hpp"

class TilemapManager : public spk::IWidget
{ 
private:
    spk::GameObject _tilemapObject;
    spk::Tilemap2D* _tilemapComponent;
    std::unique_ptr<EventSource::Type::Contract> _onUpdateVisibleChunkContract;
    bool _needActiveChunkUpdate = true;

    TilemapGenerator _tilemapGenerator;

    void _updateChunkVisibleOnScreen();

    void _onRender();

    void _generateTilemap(const spk::Vector2Int& p_tilemapSize);
    void _applyChunkValues(const TilemapGenerator::OutputFormat& p_tilemapValues);

public:
    TilemapManager(const std::string& p_name, spk::IWidget* p_parent);

    spk::GameObject& tilemapObject();
    spk::Tilemap2D* tilemapComponent();
};