#include "labi.hpp"

enum class Event
{
    OnPlayerChangingChunk,
    OnCameraChunkVisionRangeChange
};

using EventManager = spk::Singleton<spk::EventManager<Event>>;

class Chunk : public spk::GameObject
{
public:
    static inline const size_t Size = 16;
    static inline const size_t Height = 16;
    static inline spk::Vector2 OnScreenSize = 0;
    static inline spk::SpriteSheet* SpriteSheet = nullptr;

private:
    class InternalComponent : public spk::GameComponent
    {
    private:
        int8_t _data[Size][Size][Height];
        spk::MeshRenderer* _ownerMeshRenderer;
        spk::Mesh _mesh;

        void _insertTileToMesh(size_t p_x, size_t p_y, size_t p_z)
        {
            unsigned int indexes[6] = {0, 1, 2, 2, 1, 3};
            unsigned int nbVertices = _mesh.points().size();

            _mesh.addPoint(spk::Vector3(p_x + 1 - 0.5f, p_y + 1 - 0.5f, p_z));
            _mesh.addPoint(spk::Vector3(p_x + 0 - 0.5f, p_y + 1 - 0.5f, p_z));
            _mesh.addPoint(spk::Vector3(p_x + 1 - 0.5f, p_y + 0 - 0.5f, p_z));
            _mesh.addPoint(spk::Vector3(p_x + 0 - 0.5f, p_y + 0 - 0.5f, p_z));

            _mesh.addUVs(spk::Vector2(1, 0));
            _mesh.addUVs(spk::Vector2(0, 0));
            _mesh.addUVs(spk::Vector2(1, 1));
            _mesh.addUVs(spk::Vector2(0, 1));

            for (size_t i = 0; i < 6; i++)
            {
                _mesh.addIndex(indexes[i] + nbVertices, indexes[i] + nbVertices);
            }
        }

        void _bake()
        {
            _mesh.clear();

            for (size_t i = 0; i < Size; i++)
            {
                for (size_t j = 0; j < Size; j++)
                {
                    for (size_t h = 0; h < Height; h++)
                    {
                        if (_data[i][j][h] != -1)
                            _insertTileToMesh(i, j, h);
                    }
                }
            }
        }

        void _onRender()
        {

        }

        void _onUpdate()
        {

        }

    public:
        InternalComponent(const std::string& p_name) : 
            spk::GameComponent(p_name),
            _ownerMeshRenderer(owner()->getComponent<spk::MeshRenderer>())
        {
            _mesh = spk::createSpriteMesh();
            _ownerMeshRenderer->setMesh(&_mesh);
            _ownerMeshRenderer->activate();

            for (size_t i = 0; i < Size; i++)
            {
                for (size_t j = 0; j < Size; j++)
                {
                    for (size_t k = 0; k < Height; k++)
                    {
                        _data[i][j][k] = (k == 0 ? 0 : -1);
                    }
                }
            }
        }

        void setContent(const spk::Vector2Int& p_position, size_t p_height, int8_t p_value)
        {
            if (p_position.x >= 0 && p_position.x < Size &&
                p_position.y >= 0 && p_position.y < Size &&
                p_height >= 0 && p_height < Height)
            {
                _data[p_position.x][p_position.y][p_height] = p_value;
            }
        }

        int8_t content(const spk::Vector2Int& p_position, size_t p_height)
        {
            if (p_position.x >= 0 && p_position.x < Size &&
                p_position.y >= 0 && p_position.y < Size &&
                p_height >= 0 && p_height < Height)
            {
                return (_data[p_position.x][p_position.y][p_height]);
            }
            return (-1);
        }

        void launchBake()
        {
            _bake();
            _mesh.validate();
        }
    };

    spk::MeshRenderer* _meshRenderer;
    InternalComponent* _internalComponent;
    
public:
    Chunk(const spk::Vector2Int& p_position) : 
        spk::GameObject("Chunk [" + p_position.to_string() + "]")
    {
        if (SpriteSheet == nullptr)
            SpriteSheet = new spk::SpriteSheet("basicSprite.png", spk::Vector2Int(8, 6));

        transform().translation = spk::Vector3(p_position.x * static_cast<int>(Size), p_position.y * static_cast<float>(Size), 0);

        _meshRenderer = addComponent<spk::MeshRenderer>("MeshRenderer");
        _meshRenderer->setSpriteSheet(SpriteSheet);
        _meshRenderer->setSprite(spk::Vector2Int(0, 0));

        _internalComponent = addComponent<InternalComponent>("InternalComponent");
        deactivate();
    }

    void setContent(const spk::Vector2Int& p_position, size_t p_height, int8_t p_value)
    {
        _internalComponent->setContent(p_position, p_height, p_value);
    }

    int8_t content(const spk::Vector2Int& p_position, size_t p_height)
    {
        return (_internalComponent->content(p_position, p_height));
    }

    void launchBake()
    {
        _internalComponent->launchBake();
    }

    static spk::Vector2Int convertAbsoluteToRelativePosition(const spk::Vector2Int& p_position)
    {
        return (spk::Vector2Int::floor((p_position / static_cast<float>(Size))));
    }
};

class Tilemap : public spk::GameObject
{
private:
    class InternalComponent : public spk::GameComponent
    {
    private:
        spk::Vector2Int _negativeChunkRenderingOffset;
        spk::Vector2Int _positiveChunkRenderingOffset;

        std::map<spk::Vector2Int, std::unique_ptr<Chunk>> _chunks;
        std::vector<Chunk*> _activeChunks;
        std::vector<std::unique_ptr<spk::EventManager<Event>::Contract>> _subscriptionContracts;

        void _activatingVisibleChunks()
        {
            spk::Vector2Int cameraChunkPosition = convertWorldToChunkPosition(spk::Camera::mainCamera()->owner()->transform().translation.get().xy());
            spk::Vector2Int start = cameraChunkPosition + _negativeChunkRenderingOffset;
            spk::Vector2Int end = cameraChunkPosition + _positiveChunkRenderingOffset;

            std::cout << "Start : " << start << std::endl;
            std::cout << "End : " << end << std::endl;

            for (size_t i = 0; i < _activeChunks.size(); i++)
            {
                _activeChunks[i]->deactivate();
            }
            _activeChunks.clear();

            for (int i = start.x; i <= end.x; i++)
            {
                for (int j = start.y; j <= end.y; j++)
                {
                    std::unique_ptr<Chunk>& tmpChunk = requestChunk(spk::Vector2Int(i, j));
                    owner()->addChild(tmpChunk.get());
                    tmpChunk->launchBake();

                    tmpChunk->activate();

                    _activeChunks.push_back(tmpChunk.get());
                }
            }
        }

        void _onRender()
        {
            
        }   

        void _onUpdate()
        {

        }

    public:
        InternalComponent(const std::string& p_name) : 
            spk::GameComponent(p_name)
        {
            _subscriptionContracts.push_back(EventManager::instance()->subscribe(Event::OnPlayerChangingChunk, [&](){_activatingVisibleChunks();}));
            _subscriptionContracts.push_back(EventManager::instance()->subscribe(Event::OnCameraChunkVisionRangeChange, [&](){_activatingVisibleChunks();}));
        }

        void setViewRange(const spk::Vector2Int& p_negativeChunkRenderingOffset, const spk::Vector2Int& p_positiveChunkRenderingOffset)
        {
            _negativeChunkRenderingOffset = p_negativeChunkRenderingOffset;
            _positiveChunkRenderingOffset = p_positiveChunkRenderingOffset;
            EventManager::instance()->notify(Event::OnCameraChunkVisionRangeChange);
        }

        bool contain(const spk::Vector2Int& p_chunkPosition) const
        {
            return (_chunks.contains(p_chunkPosition));
        }

        std::unique_ptr<Chunk>& requestChunk(const spk::Vector2Int& p_chunkPosition)
        {
            if (_chunks.contains(p_chunkPosition) == false)
            {
                _chunks[p_chunkPosition] = std::make_unique<Chunk>(p_chunkPosition);
            }
            return (_chunks.at(p_chunkPosition));
        }

        const std::unique_ptr<Chunk>& chunk(const spk::Vector2Int& p_chunkPosition) const
        {
            if (_chunks.contains(p_chunkPosition) == false)
                throw std::runtime_error("Can't access chunk [" + p_chunkPosition.to_string() + "] : it doesn't exist");
            return (_chunks.at(p_chunkPosition));
        }
    };

    InternalComponent* _internalComponent;

public:
    Tilemap(const std::string& p_name) : 
        spk::GameObject(p_name),
        _internalComponent(addComponent<InternalComponent>("InternalComponent"))
    {

    }

    void setViewRange(const spk::Vector2Int& p_negativeChunkRenderingOffset, const spk::Vector2Int& p_positiveChunkRenderingOffset)
    {
        _internalComponent->setViewRange(p_negativeChunkRenderingOffset, p_positiveChunkRenderingOffset);

    }

    bool contain(const spk::Vector2Int& p_chunkPosition) const
    {
        return (_internalComponent->contain(p_chunkPosition));
    }

    std::unique_ptr<Chunk>& requestChunk(const spk::Vector2Int& p_chunkPosition)
    {
        return (_internalComponent->requestChunk(p_chunkPosition));
    }

    const std::unique_ptr<Chunk>& chunk(const spk::Vector2Int& p_chunkPosition) const
    {
        return (_internalComponent->chunk(p_chunkPosition));
    }

    static spk::Vector2Int convertWorldToChunkPosition(const spk::Vector2Int& p_position)
    {
        spk::Vector2 divided = static_cast<spk::Vector2>(p_position) / static_cast<float>(Chunk::Size);
        spk::Vector2Int floorized = spk::Vector2Int::floor(divided);
        std::cout << "Point : " << p_position << " convert to : " << floorized << "(" << divided << ")" << std::endl;
        return (floorized);
    }
};

class WorldManagerWidget : public spk::IWidget
{
private:
    spk::GameEngineManager _gameEngineRenderer;
    spk::GameEngine _engine;

    spk::GameObject _cameraObject;
    spk::Camera* _mainCamera;

    Tilemap _backgroundTilemap;

    spk::Vector2 convertScreenToWorldCoordinate(const spk::Vector2Int& p_screenPosition)
    {
        return (p_screenPosition / Chunk::OnScreenSize);
    }

    void _onGeometryChange()
    {
        Chunk::OnScreenSize = size() / _mainCamera->orthographicSize();
        _gameEngineRenderer.setGeometry(anchor(), size());

        spk::Vector2Int downRight = Tilemap::convertWorldToChunkPosition(convertScreenToWorldCoordinate(size() / 2.0f));
        spk::Vector2Int upLeft = Tilemap::convertWorldToChunkPosition(convertScreenToWorldCoordinate(size() / -2.0f));

        _backgroundTilemap.setViewRange(upLeft, downRight);
    }

    void _onRender()
    {

    }

    void _onUpdate()
    {

    }

public:
    WorldManagerWidget(const std::string& p_name) :
        IWidget(p_name),
        _gameEngineRenderer("GameEngineRenderer"),
        _cameraObject("CameraObject"),
        _backgroundTilemap("BackgroundTilemap")
    {
        _engine.subscribe(&_cameraObject);
        _engine.subscribe(&_backgroundTilemap);

        _cameraObject.transform().translation = spk::Vector3(0, 0, -10);
        _cameraObject.transform().rotation = spk::Quaternion::lookAt(spk::Vector3(0, 0, 0), spk::Vector3(0, 1, 0));

        _mainCamera = _cameraObject.addComponent<spk::Camera>("MainCamera");
        _mainCamera->setType(spk::Camera::Type::Orthographic);
        _mainCamera->setOrthographicSize(spk::Vector2(60, 60));
        _mainCamera->setAsMainCamera();


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