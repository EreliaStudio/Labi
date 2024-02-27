#include "tilemap_manager.hpp"

#include "texture_atlas.hpp"

void TilemapManager::_updateChunkVisibleOnScreen()
{
	if (spk::Camera::mainCamera() == nullptr)
		return ;

	spk::Vector2 nbTileOnScreen = spk::Vector2::ceiling(spk::Camera::mainCamera()->orthographicSize() / 2.0f);

	spk::Vector2Int playerPosition = spk::Camera::mainCamera()->owner()->globalPosition().xy();

	spk::Vector2 start = spk::Tilemap2D::convertWorldToChunkPosition(playerPosition - nbTileOnScreen);
	spk::Vector2 end = spk::Tilemap2D::convertWorldToChunkPosition(playerPosition + nbTileOnScreen);

	_tilemapComponent->setActiveChunkRange(start, end);

	static spk::Vector2Int chunkOffsets[8] = {
		spk::Vector2Int(-1, -1),
		spk::Vector2Int(0, -1),
		spk::Vector2Int(1, -1),

		spk::Vector2Int(-1, 0),
		spk::Vector2Int(1, 0),

		spk::Vector2Int(-1, 1),
		spk::Vector2Int(0, 1),
		spk::Vector2Int(1, 1)
	};

	for (const auto& element : _tilemapComponent->missingChunks())
	{
		spk::Tilemap2D::Chunk* newChunk = dynamic_cast<spk::Tilemap2D::Chunk*>(_tilemapComponent->createEmpyChunk(element));

		for (size_t i = 0; i < 8; i++)
		{
			spk::GameObject* chunkObject = _tilemapComponent->chunkObject(element + chunkOffsets[i]);
			if (chunkObject != nullptr)
				chunkObject->getComponent<spk::Tilemap2D::Chunk>()->bake();
		}
	}
	_tilemapComponent->updateActiveChunks();
}

void TilemapManager::_onRender()
{
	if (_needActiveChunkUpdate == true)
	{
		_updateChunkVisibleOnScreen();
		_needActiveChunkUpdate = false;
	}
}

TilemapManager::TilemapManager(const std::string& p_name, spk::IWidget* p_parent) :
	spk::IWidget(p_name, p_parent),
	_tilemapObject("TilemapObject"),
	_tilemapComponent(_tilemapObject.addComponent<spk::Tilemap2D>("Tilemap")),
	_onUpdateVisibleChunkContract(EventSource::instance()->subscribe(Event::UpdateVisibleChunk, [&](){
		_needActiveChunkUpdate = true;
	}))
{
	_tilemapObject.transform().translation = spk::Vector3(0, 0, 0);
	
	_tilemapComponent->insertNodeType(0, spk::Tilemap2D::Node(spk::Vector2Int(0, 0), spk::Tilemap2D::Node::OBSTACLE, true));
	_tilemapComponent->insertNodeType(1, spk::Tilemap2D::Node(spk::Vector2Int(4, 0), spk::Tilemap2D::Node::WALKABLE, false));
	
	_tilemapComponent->setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("WorldTileset"));
}

spk::GameObject& TilemapManager::tilemapObject()
{
	return (_tilemapObject);
}

spk::Tilemap2D* TilemapManager::tilemapComponent()
{
	return (_tilemapComponent);
}