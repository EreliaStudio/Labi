#include "tilemap_manager.hpp"

#include "texture_atlas.hpp"
#include "tilemap_generator.hpp"

void TilemapManager::_updateChunkVisibleOnScreen()
{
	if (spk::Camera::mainCamera() == nullptr)
		return ;

	spk::Vector2 nbTileOnScreen = spk::Vector2::ceiling(spk::Camera::mainCamera()->orthographicSize() / 2.0f);

	spk::Vector2Int playerPosition = spk::Camera::mainCamera()->owner()->globalPosition().xy();

	spk::Vector2 start = spk::Tilemap2D::convertWorldToChunkPosition(playerPosition - nbTileOnScreen - 1);
	spk::Vector2 end = spk::Tilemap2D::convertWorldToChunkPosition(playerPosition + nbTileOnScreen + 1);

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

void TilemapManager::_applyChunkValues(const TilemapGenerator::OutputFormat& p_tilemapValues)
{
	if (p_tilemapValues.size() == 0)
		return ;

	spk::Vector2Int tilemapSize = spk::Vector2Int(p_tilemapValues.size() / spk::Tilemap2D::Chunk::SizeX, p_tilemapValues[0].size() / spk::Tilemap2D::Chunk::SizeY);

	for (int i = 0; i < tilemapSize.x; i++)
	{
		for (int j = 0; j < tilemapSize.y; j++)
		{
			spk::Tilemap2D::Chunk* newChunk = dynamic_cast<spk::Tilemap2D::Chunk*>(_tilemapComponent->createEmpyChunk(spk::Vector2Int(i, j)));

			for (int x = 0; x < spk::Tilemap2D::Chunk::SizeX; x++)
			{
				for (int y = 0; y < spk::Tilemap2D::Chunk::SizeY; y++)
				{
					newChunk->setContent(x, y, 0, p_tilemapValues[i * spk::Tilemap2D::Chunk::SizeX + x][ j * spk::Tilemap2D::Chunk::SizeY + y]);
				}  
			}
		}
	}
}

void TilemapManager::_generateTilemap(const spk::Vector2Int& p_tilemapSize)
{
	TilemapGenerator::OutputFormat tilemapValues = _tilemapGenerator.generate(TilemapGenerator::Type::Cave, spk::Vector2Int(12, 12));

	_applyChunkValues(tilemapValues);
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
	  
	_tilemapComponent->setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("worldBackgound"));

	_generateTilemap(spk::Vector2Int(12, 12));
}

spk::GameObject& TilemapManager::tilemapObject()
{
	return (_tilemapObject);
}

spk::Tilemap2D* TilemapManager::tilemapComponent()
{
	return (_tilemapComponent);
}