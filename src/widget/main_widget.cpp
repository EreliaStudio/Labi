#include "main_widget.hpp"
#include "miscellaneous/texture_atlas.hpp"

#include "miscellaneous/event_source.hpp"
#include "miscellaneous/texture_atlas.hpp"
#include "miscellaneous/context.hpp"

void MainWidget::_onGeometryChange()
{
	_gameEngineManager.setGeometry(anchor(), size());
	Context::instance()->playerObject.camera().setOrthographicSize(static_cast<spk::Vector2>(size()) / 32.0f);
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
	if (spk::Application::activeApplication()->keyboard().getKey(spk::Keyboard::Space) == spk::InputState::Pressed)
		EventSource::instance()->notify_all(Event::ExitBattleMode);
}

std::map<spk::Vector2Int, bool> computeCircleArea(const float& p_radius)
{
	static spk::Vector2Int deltaPosition[4] = {
		spk::Vector2Int(1, 0),
		spk::Vector2Int(-1, 0),
		spk::Vector2Int(0, 1),
		spk::Vector2Int(0, -1)
	};
	std::map<spk::Vector2Int, bool> result;
	float squaredDistance = p_radius * p_radius;

	for (int i = -p_radius; i <= p_radius; i++)
	{
		for (int j = -p_radius; j <= p_radius; j++)
		{
			spk::Vector2Int tmpPosition = spk::Vector2Int(i, j);

			if (tmpPosition.distanceSquared(0) <= squaredDistance + 1.0f)
			{
				result[tmpPosition] = false;
			}
		}
	}

	for (auto&[key, element] : result)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (result.contains(key + deltaPosition[i]) == false)
				element = true;
		}
	}

	return (result);
}

void MainWidget::_populateBattleArea()
{
	spk::Vector2Int playerPosition = spk::Vector2Int::floor(Context::instance()->playerObject.globalPosition().xy());

	static std::map<spk::Vector2Int, bool> circleArea = computeCircleArea(10.0f);
	
	for (auto& [key, element] : circleArea)
	{
		spk::Vector2Int absolutePosition = key + playerPosition;

		if (element == true)
			Context::instance()->battleAreaObject.addBorderTile(absolutePosition);
		else
		{	
			Context::instance()->battleAreaObject.addBattleTile(
					absolutePosition,
					(Context::instance()->tilemap.isObstacle(absolutePosition) == true ? BattleArenaObject::TileObject::Type::Obstacle : BattleArenaObject::TileObject::Type::Standard)
				);
		}
	}

	Context::instance()->battleAreaObject.generateStartingPosition(BattleArenaObject::TileObject::Type::AllyStartingPosition, 5);
	Context::instance()->battleAreaObject.generateStartingPosition(BattleArenaObject::TileObject::Type::EnemyStartingPosition, 5); 
}

void MainWidget::_movePlayer(const spk::Vector3& p_deltaPosition, const long long& p_duration)
{
	spk::Vector3 nextPlace = Context::instance()->playerObject.globalPosition() + p_deltaPosition;
	if (Context::instance()->tilemap.isObstacle(nextPlace) == false) 
	{
		if (Context::instance()->tilemap.isFlag(nextPlace, BUSH) == true && _bushFightGenerator.sample() < 100)
		{
			Context::instance()->playerObject.transform().translation += p_deltaPosition;
			EventSource::instance()->notify_all(Event::EnterBattleMode);
		}
		else
		{
			Context::instance()->playerObject.requestMotion(p_deltaPosition, p_duration);
		}	
	}
}

void MainWidget::_enterBattleMode()
{
	_populateBattleArea();
	Context::instance()->playerObject.setMode(PlayerObject::Mode::Battle);
	Context::instance()->battleAreaObject.activate();
}

void MainWidget::_exitBattleMode()
{
	Context::instance()->playerObject.setMode(PlayerObject::Mode::World);
	Context::instance()->battleAreaObject.deactivate();
}

MainWidget::MainWidget(const std::string& p_name) :
	spk::IWidget(p_name),
	_singletonInstanciator(),
	_gameEngineManager("GameEngineManager", this), 
	_onChunkUpdateContract(EventSource::instance()->subscribe(Event::UpdateVisibleChunkRequest, [&](){
		spk::Vector2 nbTileOnScreen = static_cast<spk::Vector2>(size()) / static_cast<spk::Vector2>(spk::Camera::mainCamera()->orthographicSize());
		spk::Vector2Int nbChunkOnScreen = spk::Tilemap2D::convertWorldToChunkPosition(nbTileOnScreen) / 2.0f + 1;
		spk::Vector2Int playerChunkPosition = spk::Tilemap2D::convertWorldToChunkPositionXY(Context::instance()->playerObject.globalPosition());

		spk::Vector2Int first = playerChunkPosition - nbChunkOnScreen;
		spk::Vector2Int second = playerChunkPosition + nbChunkOnScreen;

		Context::instance()->tilemap.setActiveChunkRange(first, second);
		for (auto& chunkMissing : Context::instance()->tilemap.missingChunks())
		{
			std::filesystem::path chunkFile = "resources/chunk/chunk_" + std::to_string(chunkMissing.x) + "_" + std::to_string(chunkMissing.y) + ".chk";
 
			if (std::filesystem::exists(chunkFile)) 
			{
				spk::Tilemap2D::IChunk* newChunk = Context::instance()->tilemap.createChunk(chunkMissing); 
				newChunk->loadFromFile(chunkFile);
			}
		}
		Context::instance()->tilemap.updateActiveChunks();
	})),
	_playerMotionContracts{
		EventSource::instance()->subscribe(Event::PlayerMotionUp,	 [&](){_movePlayer(spk::Vector3( 0,  1, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionLeft,  [&](){_movePlayer(spk::Vector3(-1,  0, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionDown,  [&](){_movePlayer(spk::Vector3( 0, -1, 0), 150);}),
		EventSource::instance()->subscribe(Event::PlayerMotionRight, [&](){_movePlayer(spk::Vector3( 1,  0, 0), 150);})
	},
	_enteringBattleModeContract(EventSource::instance()->subscribe(Event::EnterBattleMode, [&](){_enterBattleMode();})),
	_exitingBattleModeContract(EventSource::instance()->subscribe(Event::ExitBattleMode, [&](){_exitBattleMode();}))
{
	_bushFightGenerator.configureRange(0, 100);

	Context::instance()->playerObject.transform().translation = spk::Vector3(0, 0, 2.5f);

	Context::instance()->tilemap.setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("ChunkSpriteSheet"));
	_loadTilemapNode();

	Context::instance()->battleAreaObject.deactivate();

	Context::instance()->gameEngine.subscribe(&(Context::instance()->playerObject));
	Context::instance()->gameEngine.subscribe(&(Context::instance()->tilemap)); 
	Context::instance()->gameEngine.subscribe(&Context::instance()->battleAreaObject); 

	_gameEngineManager.setGameEngine(&(Context::instance()->gameEngine));
	_gameEngineManager.activate(); 

	_exitBattleMode();
}

MainWidget::~MainWidget()
{

}