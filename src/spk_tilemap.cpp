#include "spk_tilemap.hpp"
#include "game_engine/component/spk_camera_component.hpp"

namespace spk
{
	void Tilemap::InternalComponent::_activatingVisibleChunks()
	{
		spk::Vector2Int cameraChunkPosition = convertWorldToChunkPosition(spk::Camera::mainCamera()->owner()->transform().translation.get().xy());
		spk::Vector2Int start = cameraChunkPosition + _negativeChunkRenderingOffset;
		spk::Vector2Int end = cameraChunkPosition + _positiveChunkRenderingOffset;

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
				tmpChunk->launchBake(_nodeMap);

				tmpChunk->activate();

				_activeChunks.push_back(tmpChunk.get());
			}
		}
	}

	void Tilemap::InternalComponent::_onRender()
	{
		
	}   

	void Tilemap::InternalComponent::_onUpdate()
	{

	}

	Tilemap::InternalComponent::InternalComponent(const std::string& p_name) : 
		spk::GameComponent(p_name)
	{

	}

	Tilemap::NodeMap& Tilemap::InternalComponent::nodeMap()
	{
		return (_nodeMap);
	}
	const Tilemap::NodeMap& Tilemap::InternalComponent::nodeMap() const
	{
		return (_nodeMap);
	}

	void Tilemap::InternalComponent::setViewRange(const spk::Vector2Int& p_negativeChunkRenderingOffset, const spk::Vector2Int& p_positiveChunkRenderingOffset)
	{
		_negativeChunkRenderingOffset = p_negativeChunkRenderingOffset;
		_positiveChunkRenderingOffset = p_positiveChunkRenderingOffset;
	}

	void Tilemap::InternalComponent::updateVisibleChunks()
	{
		_activatingVisibleChunks();
	}

	bool Tilemap::InternalComponent::contain(const spk::Vector2Int& p_chunkPosition) const
	{
		return (_chunks.contains(p_chunkPosition));
	}

	std::unique_ptr<Chunk>& Tilemap::InternalComponent::requestChunk(const spk::Vector2Int& p_chunkPosition)
	{
		if (_chunks.contains(p_chunkPosition) == false)
		{
			_chunks[p_chunkPosition] = std::make_unique<Chunk>(p_chunkPosition);
		}
		return (_chunks.at(p_chunkPosition));
	}

	const std::unique_ptr<Chunk>& Tilemap::InternalComponent::chunk(const spk::Vector2Int& p_chunkPosition) const
	{
		if (_chunks.contains(p_chunkPosition) == false)
			throw std::runtime_error("Can't access chunk [" + p_chunkPosition.to_string() + "] : it doesn't exist");
		return (_chunks.at(p_chunkPosition));
	}

    Tilemap::Tilemap(const std::string& p_name) : 
        spk::GameObject(p_name),
        _internalComponent(addComponent<InternalComponent>("InternalComponent"))
    {

    }

	Tilemap::NodeMap& Tilemap::nodeMap()
	{
		return (_internalComponent->nodeMap());
	}
	const Tilemap::NodeMap& Tilemap::nodeMap() const
	{
		return (_internalComponent->nodeMap());
	}

	void Tilemap::updateVisibleChunks()
	{
		_internalComponent->updateVisibleChunks();
	}

    void Tilemap::setViewRange(const spk::Vector2Int& p_negativeChunkRenderingOffset, const spk::Vector2Int& p_positiveChunkRenderingOffset)
    {
        _internalComponent->setViewRange(p_negativeChunkRenderingOffset, p_positiveChunkRenderingOffset);
    }

    bool Tilemap::contain(const spk::Vector2Int& p_chunkPosition) const
    {
        return (_internalComponent->contain(p_chunkPosition));
    }

    std::unique_ptr<Chunk>& Tilemap::requestChunk(const spk::Vector2Int& p_chunkPosition)
    {
        return (_internalComponent->requestChunk(p_chunkPosition));
    }

    const std::unique_ptr<Chunk>& Tilemap::chunk(const spk::Vector2Int& p_chunkPosition) const
    {
        return (_internalComponent->chunk(p_chunkPosition));
    }

    spk::Vector2Int Tilemap::convertWorldToChunkPosition(const spk::Vector2Int& p_position)
    {
        return (spk::Vector2Int::floor(static_cast<spk::Vector2>(p_position) / static_cast<float>(Chunk::Size)));
    }
}