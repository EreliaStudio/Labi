#include "main_widget.hpp"
#include "texture_atlas.hpp"

void MainWidget::_onGeometryChange()
{
	_gameEngineManager.setGeometry(anchor(), size());
	_playerController.setGeometry(anchor(), size());
	_tilemapManager.setGeometry(anchor(), size());
}

MainWidget::MainWidget(const std::string& p_name) :
	spk::IWidget(p_name),
	_singletonInstanciator(),
	_gameEngineManager("GameEngineManager", this),
	_playerController("PlayerController", this),
	_tilemapManager("TilemapManager", this)
{

	_playerController.playerBodyRenderer()->setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("PlayerSprite"));

	_gameEngine.subscribe(&(_playerController.playerObject()));
	_gameEngine.subscribe(&(_tilemapManager.tilemapObject()));

	_gameEngineManager.setGameEngine(&_gameEngine);
	_gameEngineManager.activate(); 
	_playerController.activate(); 
	_tilemapManager.activate();
}

MainWidget::~MainWidget()
{

}