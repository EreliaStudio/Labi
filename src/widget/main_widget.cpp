#include "main_widget.hpp"
#include "miscellaneous/texture_atlas.hpp"

#include "miscellaneous/event_source.hpp"
#include "miscellaneous/texture_atlas.hpp"
#include "miscellaneous/context.hpp"

void MainWidget::_onGeometryChange()
{
	_gameEngineManager.setGeometry(anchor(), size());
	_playerObject.camera().setOrthographicSize(static_cast<spk::Vector2>(size()) / 32.0f);
	EventSource::instance()->notify_all(Event::UpdateVisibleChunkRequest);
}

void insertNewNode(std::vector<spk::Vector2Int>& p_selectedTile, std::vector<spk::Vector2Int>& p_toCalc, const spk::Vector2Int& p_position)
{
	static const spk::Vector2Int deltaPosition[4] = {
		spk::Vector2Int(1, 0),
		spk::Vector2Int(-1, 0),
		spk::Vector2Int(0, 1),
		spk::Vector2Int(0, -1)
	};

	p_selectedTile.push_back(p_position);
	for (size_t i = 0; i < 4; i++)
	{
		spk::Vector2Int nextPosition = p_position + deltaPosition[i];
		if (std::find(p_toCalc.begin(), p_toCalc.end(), nextPosition) == p_toCalc.end())
			p_toCalc.push_back(nextPosition);
	}
}

void MainWidget::_onUpdate()
{
	if (spk::Application::activeApplication()->keyboard().getKey(spk::Keyboard::Space) == spk::InputState::Down)
		_playerObject.setMode(PlayerObject::Mode::World);
}

void MainWidget::_generateFightArea()
{
	spk::Vector2Int playerPosition = spk::Vector2Int::floor(_playerObject.globalPosition().xy());
	std::vector<spk::Vector2Int> selectedTiles;
	std::vector<spk::Vector2Int> toCalc = {playerPosition};

	size_t index = 0;
	while (toCalc.size() > index)
	{
		if (std::find(selectedTiles.begin(), selectedTiles.end(), toCalc[index]) == selectedTiles.end() &&
			playerPosition.distance(toCalc[index]) <= 2 &&
			_tilemap2D.isObstacle(toCalc[index]) == false)
		{
			insertNewNode(selectedTiles, toCalc, toCalc[index]);
		}
		index++;
	}

	std::cout << "Selected tile : ";
	for (size_t i = 0; i < selectedTiles.size(); i++)
	{
		if (i != 0)
			std::cout << " - ";
		std::cout << "[" << selectedTiles[i] << "]";
	}
	std::cout << std::endl;
}

void MainWidget::_movePlayer(const spk::Vector3& p_deltaPosition, const long long& p_duration)
{
	spk::Vector3 nextPlace = _playerObject.globalPosition() + p_deltaPosition;
	if (_tilemap2D.isObstacle(nextPlace) == false) 
	{
		if (_tilemap2D.isFlag(nextPlace, BUSH) == true && _bushFightGenerator.sample() < 100)
		{
			_playerObject.setMode(PlayerObject::Mode::Battle);
			_playerObject.transform().translation += p_deltaPosition;
			_generateFightArea();
		}
		else
		{
			_playerObject.requestMotion(p_deltaPosition, p_duration);
		}	
	}
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
	})),
	_notifierContracts{
		EventSource::instance()->subscribe(Event::PlayerMotionUp,	 [&](){_movePlayer(spk::Vector3( 0,  1, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionLeft,  [&](){_movePlayer(spk::Vector3(-1,  0, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionDown,  [&](){_movePlayer(spk::Vector3( 0, -1, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionRight, [&](){_movePlayer(spk::Vector3( 1,  0, 0), 150);})
	}
{
	Context::instance()->playerObject = &_playerObject;
	Context::instance()->tilemap = &_tilemap2D;

	_bushFightGenerator.configureRange(0, 100);

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