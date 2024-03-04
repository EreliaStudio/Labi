#include "main_widget.hpp"
#include "miscellaneous/texture_atlas.hpp"

void MainWidget::_onGeometryChange()
{
	_gameEngineManager.setGeometry(anchor(), size());
	_playerObject.camera().setOrthographicSize(static_cast<spk::Vector2>(size()) / 32.0f);
	EventSource::instance()->notify_all(Event::UpdateVisibleChunkRequest);
}

MainWidget::MainWidget(const std::string& p_name) :
	spk::IWidget(p_name),
	_singletonInstanciator(),
	_gameEngineManager("GameEngineManager", this), 
	_playerObject("Player"),
	_tilemap2D("Tilemap"),
	_onChunkUpdateContract(EventSource::instance()->subscribe(Event::UpdateVisibleChunkRequest, [&](){
		spk::Vector2 nbTileOnScreen = static_cast<spk::Vector2>(size()) / static_cast<spk::Vector2>(spk::Camera::mainCamera()->orthographicSize());
		spk::Vector2Int nbChunkOnScreen = spk::Tilemap2D::convertWorldToChunkPosition(nbTileOnScreen) / 2.0f + 1;
		spk::Vector2Int playerChunkPosition = spk::Tilemap2D::convertWorldToChunkPositionXY(_playerObject.globalPosition());

		spk::Vector2Int first = playerChunkPosition - nbChunkOnScreen;
		spk::Vector2Int second = playerChunkPosition + nbChunkOnScreen;

		_tilemap2D.setActiveChunkRange(first, second);
		for (auto& chunkMissing : _tilemap2D.missingChunks())
		{
			std::filesystem::path chunkFile = "resources/chunk/chunk_" + std::to_string(chunkMissing.x) + "_" + std::to_string(chunkMissing.y) + ".chk";
 
			if (std::filesystem::exists(chunkFile)) 
			{
				spk::Tilemap2D::IChunk* newChunk = _tilemap2D.createChunk(chunkMissing); 
				newChunk->loadFromFile(chunkFile);
			}
		}
		_tilemap2D.updateActiveChunks();
	})) 
{
	_playerObject.transform().translation = spk::Vector3(0, 0, 2.5f);

	_tilemap2D.setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("ChunkSpriteSheet"));
	_loadTilemapNode();

	_gameEngine.subscribe(&_playerObject);
	_gameEngine.subscribe(&_tilemap2D); 

	_gameEngineManager.setGameEngine(&_gameEngine);
	_gameEngineManager.activate(); 

}

MainWidget::~MainWidget()
{

}