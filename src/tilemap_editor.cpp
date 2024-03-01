#include "tilemap_editor.hpp"

void TilemapEditor::_onUpdate()
{
	if (spk::Application::activeApplication()->timeManager().deltaTime() == 0)
		return;

	static const spk::Vector2Int chunkOffset[9] = {
		spk::Vector2Int(-1, -1),		
		spk::Vector2Int(0, -1),		
		spk::Vector2Int(1, -1),

		spk::Vector2Int(-1, 0),		
		spk::Vector2Int(0, 0),		
		spk::Vector2Int(1, 0),		

		spk::Vector2Int(-1, 1),		
		spk::Vector2Int(0, 1),		
		spk::Vector2Int(1, 1)
	};

	if (spk::Application::activeApplication()->mouse().getButton(spk::Mouse::Left) == spk::InputState::Down)
	{
		spk::Vector2Int mouseWorldPosition = spk::Vector2::floor(spk::Camera::convertScreenToWorldPosition2D(spk::Application::activeApplication()->mouse().position(), anchor(), size()));

		spk::Vector2Int chunkPosition = _tilemap->convertWorldToChunkPosition(mouseWorldPosition);

		if (_tilemap->containsChunk(chunkPosition) == true)
		{
			spk::Tilemap2D::Chunk* chunk = _tilemap->chunkObject(chunkPosition)->getComponent<spk::Tilemap2D::Chunk>();

			spk::Vector2Int relativePosition = chunk->convertAbsoluteToRelativePosition(mouseWorldPosition);

			if (chunk->content(relativePosition, 0) != 1)
			{
				chunk->setContent(relativePosition, 0, 1);

				for (size_t i = 0; i < 9; i++)
				{
					spk::GameObject* chunkObject = _tilemap->chunkObject(chunkPosition + chunkOffset[i]);
					if (chunkObject != nullptr)
						chunkObject->getComponent<spk::Tilemap2D::Chunk>()->bake();
				}
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