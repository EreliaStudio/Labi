#pragma once

#include "sparkle.hpp"

class TilemapEditor : public spk::IWidget
{
private:
	spk::Tilemap2D* _tilemap;

	size_t selectedValue = 0;
	short values[8] = {415, 416, 418, 495, 799, 5, 6, 7};

	void _onUpdate();

public:
	TilemapEditor(const std::string& p_name, spk::IWidget* p_parent);

	void setTilemap(spk::Tilemap2D* p_tilemap);
	spk::Tilemap2D* tilemap();
};