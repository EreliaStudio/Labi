#pragma once

#include "sparkle.hpp"

class TilemapEditor : public spk::IWidget
{
private:
	spk::Tilemap2D* _tilemap;

	void _onUpdate();

public:
	TilemapEditor(const std::string& p_name, spk::IWidget* p_parent);

	void setTilemap(spk::Tilemap2D* p_tilemap);
	spk::Tilemap2D* tilemap();
};