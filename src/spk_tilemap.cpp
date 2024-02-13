#include "spk_tilemap.hpp"
#include "game_engine/component/spk_camera_component.hpp"

namespace spk
{
	void Tilemap::InternalComponent::_activatingVisibleChunks()
	{
		spk::Vector2Int start = _negativeChunkRenderingOffset;
		spk::Vector2Int end = _positiveChunkRenderingOffset;

		for (size_t i = 0; i < _activeChunks.size(); i++)
		{
			_activeChunks[i]->deactivate();
		}
		_activeChunks.clear();

		for (int i = start.x; i <= end.x; i++) 
		{
			for (int j = start.y; j <= end.y; j++)
			{
				spk::Vector2Int chunkPosition = spk::Vector2Int(i, j);				
				if (_chunks.contains(chunkPosition) == true)
				{
					std::unique_ptr<Chunk>& tmpChunk = _chunks.at(spk::Vector2Int(i, j));

					owner()->addChild(tmpChunk.get());
					tmpChunk->launchBake(dynamic_cast<const Tilemap*>(owner()));

					tmpChunk->activate();

					_activeChunks.push_back(tmpChunk.get());
				}
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

	std::map<spk::Vector2Int, std::unique_ptr<Chunk>>& Tilemap::InternalComponent::chunks()
	{
		return (_chunks);
	}

	const std::map<spk::Vector2Int, std::unique_ptr<Chunk>>& Tilemap::InternalComponent::chunks() const
	{
		return (_chunks);
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

	std::map<spk::Vector2Int, std::unique_ptr<Chunk>>& Tilemap::chunks()
	{
		return (_internalComponent->chunks());
	}

	const std::map<spk::Vector2Int, std::unique_ptr<Chunk>>& Tilemap::chunks() const
	{
		return (_internalComponent->chunks());
	}

    spk::Vector2Int Tilemap::convertWorldToChunkPosition(const spk::Vector2Int& p_position)
    {
        return (spk::Vector2Int::floor(static_cast<spk::Vector2>(p_position) / static_cast<float>(Chunk::Size)));
    }
}