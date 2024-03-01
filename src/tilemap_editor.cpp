#include "tilemap_editor.hpp"

void TilemapEditor::_onUpdate()
{
	spk::Keyboard::Key valueKeys[8] = {
		spk::Keyboard::Key1,
		spk::Keyboard::Key2,
		spk::Keyboard::Key3,
		spk::Keyboard::Key4,
		spk::Keyboard::Key5,
		spk::Keyboard::Key6,
		spk::Keyboard::Key7,
		spk::Keyboard::Key8
	};

	for (size_t i = 0; i < 8; i++)
	{
		if (spk::Application::activeApplication()->keyboard().getKey(valueKeys[i]) == spk::InputState::Pressed)
		{
			selectedValue = i;
		}
	}

	if (spk::Application::activeApplication()->mouse().getButton(spk::Mouse::Left) == spk::InputState::Down)
	{
		spk::Vector2Int mouseWorldPosition = spk::Vector2::floor(spk::Camera::convertScreenToWorldPosition2D(spk::Application::activeApplication()->mouse().position(), anchor(), size()));

		spk::Vector2Int chunkPosition = _tilemap->convertWorldToChunkPosition(mouseWorldPosition);

		if (_tilemap->containsChunk(chunkPosition) == true)
		{
			spk::Tilemap2D::Chunk* chunk = _tilemap->chunkObject(chunkPosition)->getComponent<spk::Tilemap2D::Chunk>();

			spk::Vector2Int relativePosition = chunk->convertAbsoluteToRelativePosition(mouseWorldPosition);

			if (chunk->content(relativePosition, 0) != values[selectedValue])
			{
				chunk->setContent(relativePosition, 1, values[selectedValue]);

				_tilemap->rebakeChunk(chunkPosition);
			} 
		}
	}
	
	if (spk::Application::activeApplication()->mouse().getButton(spk::Mouse::Right) == spk::InputState::Down)
	{
		spk::Vector2Int mouseWorldPosition = spk::Vector2::floor(spk::Camera::convertScreenToWorldPosition2D(spk::Application::activeApplication()->mouse().position(), anchor(), size()));

		spk::Vector2Int chunkPosition = _tilemap->convertWorldToChunkPosition(mouseWorldPosition);

		if (_tilemap->containsChunk(chunkPosition) == true)
		{
			spk::Tilemap2D::Chunk* chunk = _tilemap->chunkObject(chunkPosition)->getComponent<spk::Tilemap2D::Chunk>();

			spk::Vector2Int relativePosition = chunk->convertAbsoluteToRelativePosition(mouseWorldPosition);

			if (chunk->content(relativePosition, 0) != -1)
			{
				chunk->setContent(relativePosition, 1, -1);

				_tilemap->rebakeChunk(chunkPosition);
			} 
		}
	}
}

TilemapEditor::TilemapEditor(const std::string& p_name, spk::IWidget* p_parent) :
	spk::IWidget(p_name, p_parent),
	_tilemap(nullptr)
{

}

void TilemapEditor::setTilemap(spk::Tilemap2D* p_tilemap)
{
	_tilemap = p_tilemap;
}

spk::Tilemap2D* TilemapEditor::tilemap()
{
	return (_tilemap);
}