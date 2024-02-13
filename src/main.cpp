#include "labi.hpp"

#include "spk_tilemap.hpp"

enum class Event
{
    OnPlayerChangingChunk,
    OnCameraChunkVisionRangeChange
};

using EventManager = spk::Singleton<spk::EventManager<Event>>;

class WorldManagerWidget : public spk::IWidget
{
private:
    spk::GameEngineManager _gameEngineRenderer;
    spk::GameEngine _engine;

    spk::GameObject _cameraObject;
    spk::Camera* _mainCamera;

    spk::Tilemap _backgroundTilemap;

	std::vector<std::unique_ptr<spk::EventManager<Event>::Contract>> _subscriptionContracts;

    spk::Vector2 convertScreenToWorldCoordinate(const spk::Vector2Int& p_screenPosition)
    {
        return (p_screenPosition / spk::Chunk::OnScreenSize);
    }

    void _onGeometryChange()
    {
        _mainCamera->setOrthographicSize(spk::Vector2(25 * (static_cast<float>(size().x) / static_cast<float>(size().y)), 25));
        spk::Chunk::OnScreenSize = size() / _mainCamera->orthographicSize();
        _gameEngineRenderer.setGeometry(anchor(), size());

        spk::Vector2Int downRight = spk::Tilemap::convertWorldToChunkPosition(convertScreenToWorldCoordinate(size() / 2.0f));
        spk::Vector2Int upLeft = spk::Tilemap::convertWorldToChunkPosition(convertScreenToWorldCoordinate(size() / -2.0f));

        _backgroundTilemap.setViewRange(upLeft, downRight);

        EventManager::instance()->notify(Event::OnCameraChunkVisionRangeChange);
    }

    void _onRender()
    {

    }

    void _onUpdate()
    {

    }

    void _loadNodes()
    {
        _backgroundTilemap.nodeMap()[0] = spk::Node(spk::Vector2Int(0, 0), spk::Vector2Int(0, 0), false);
    }

public:
    WorldManagerWidget(const std::string& p_name) :
        IWidget(p_name),
        _gameEngineRenderer("GameEngineRenderer"),
        _cameraObject("CameraObject"),
        _backgroundTilemap("BackgroundTilemap")
    {
        _loadNodes();
        
        _engine.subscribe(&_cameraObject);
        _engine.subscribe(&_backgroundTilemap);

        _cameraObject.transform().translation = spk::Vector3(0, 0, -10);
        _cameraObject.transform().rotation = spk::Quaternion::lookAt(spk::Vector3(0, 0, 0), spk::Vector3(0, 1, 0));

        _mainCamera = _cameraObject.addComponent<spk::Camera>("MainCamera");
        _mainCamera->setType(spk::Camera::Type::Orthographic);
        _mainCamera->setAsMainCamera();

        _subscriptionContracts.push_back(EventManager::instance()->subscribe(Event::OnCameraChunkVisionRangeChange, [&](){_backgroundTilemap.updateVisibleChunks();}));
        _subscriptionContracts.push_back(EventManager::instance()->subscribe(Event::OnPlayerChangingChunk, [&](){_backgroundTilemap.updateVisibleChunks();}));

        EventManager::instance()->notify(Event::OnPlayerChangingChunk);

        addChild(&_gameEngineRenderer);
        _gameEngineRenderer.setGameEngine(&_engine);
        _gameEngineRenderer.activate();
    }
};

class MainWidget : public spk::IWidget
{
private:
    WorldManagerWidget _worldManagerWidget;

    void _onGeometryChange()
    {
        _worldManagerWidget.setGeometry(anchor(), size());
    }

    void _onRender()
    {

    }

    void _onUpdate()
    {

    }

public:
    MainWidget(const std::string& p_name) :
        IWidget(p_name),
        _worldManagerWidget("WorldManagerWidget")
    {
        addChild(&_worldManagerWidget);
        _worldManagerWidget.activate();
    }

    ~MainWidget()
    {
        EventManager::release();
    }
};

int main()
{
    spk::Application app = spk::Application("Labi", spk::Vector2UInt(800, 800), spk::Application::Mode::Multithread);

    EventManager::instanciate();

    MainWidget mainWidget = MainWidget("MainWidget");
    mainWidget.setGeometry(0, app.size());
    mainWidget.activate();

    return (app.run());
}