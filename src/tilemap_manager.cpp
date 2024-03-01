#include "tilemap_manager.hpp"

#include "texture_atlas.hpp"

void TilemapManager::_updateChunkVisibleOnScreen()
{
	if (spk::Camera::mainCamera() == nullptr)
		return;

	spk::Vector2 nbTileOnScreen = spk::Vector2::ceiling(spk::Camera::mainCamera()->orthographicSize() / 2.0f);

	spk::Vector2Int playerPosition = spk::Camera::mainCamera()->owner()->globalPosition().xy();

	spk::Vector2 start = spk::Tilemap2D::convertWorldToChunkPosition(playerPosition - nbTileOnScreen - 1);
	spk::Vector2 end = spk::Tilemap2D::convertWorldToChunkPosition(playerPosition + nbTileOnScreen + 1);

	_tilemapComponent->setActiveChunkRange(start, end);

	for (auto& chunkMissing : _tilemapComponent->missingChunks())
	{
		std::filesystem::path chunkFile = "resources/chunk/chunk_" + std::to_string(chunkMissing.x) + "_" + std::to_string(chunkMissing.y) + ".chk";

		if (std::filesystem::exists(chunkFile))
		{
			spk::Tilemap2D::IChunk* newChunk = _tilemapComponent->createEmpyChunk(chunkMissing);
			newChunk->loadFromFile(chunkFile);
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

TilemapManager::TilemapManager(const std::string &p_name, spk::IWidget *p_parent) :
	spk::IWidget(p_name, p_parent),
	_tilemapObject("TilemapObject"),
	_tilemapComponent(_tilemapObject.addComponent<spk::Tilemap2D>("Tilemap")),
	_onUpdateVisibleChunkContract(EventSource::instance()->subscribe(Event::UpdateVisibleChunk, [&]()
	{
		_needActiveChunkUpdate = true;
	}))
{
	_tilemapObject.transform().translation = spk::Vector3(0, 0, 0);

	_loadNodes();

	_tilemapComponent->setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("ChunkSpriteSheet"));
}

TilemapManager::~TilemapManager()
{
	_tilemapComponent->saveToFolder("resources/chunk");
}

spk::GameObject &TilemapManager::tilemapObject()
{
	return (_tilemapObject);
}

spk::Tilemap2D *TilemapManager::tilemapComponent()
{
	return (_tilemapComponent);
}