#pragma once

#include "math/spk_vector2.hpp"
#include "design_pattern/spk_singleton.hpp"
#include "spk_node.hpp"
#include "spk_chunk.hpp"

namespace spk
{
	class Tilemap : public spk::GameObject
	{
	public:
		using NodeMap = std::map<Node::IndexType, Node>;

	private:
		class InternalComponent : public spk::GameComponent
		{
		private:
			spk::Vector2Int _negativeChunkRenderingOffset;
			spk::Vector2Int _positiveChunkRenderingOffset;

			std::map<spk::Vector2Int, std::unique_ptr<Chunk>> _chunks;
			std::vector<Chunk*> _activeChunks;
			NodeMap _nodeMap;

			void _activatingVisibleChunks();

			void _onRender();
			void _onUpdate();

		public:
			InternalComponent(const std::string& p_name);

			NodeMap& nodeMap();
			const NodeMap& nodeMap() const;

			void updateVisibleChunks();

			void setViewRange(const spk::Vector2Int& p_negativeChunkRenderingOffset, const spk::Vector2Int& p_positiveChunkRenderingOffset);

			bool contain(const spk::Vector2Int& p_chunkPosition) const;
			std::unique_ptr<Chunk>& requestChunk(const spk::Vector2Int& p_chunkPosition);
			const std::unique_ptr<Chunk>& chunk(const spk::Vector2Int& p_chunkPosition) const;
		};

		InternalComponent* _internalComponent;

	public:
		Tilemap(const std::string& p_name);

		NodeMap& nodeMap();
		const NodeMap& nodeMap() const;

		void updateVisibleChunks();

		void setViewRange(const spk::Vector2Int& p_negativeChunkRenderingOffset, const spk::Vector2Int& p_positiveChunkRenderingOffset);

		bool contain(const spk::Vector2Int& p_chunkPosition) const;
		std::unique_ptr<Chunk>& requestChunk(const spk::Vector2Int& p_chunkPosition);
		const std::unique_ptr<Chunk>& chunk(const spk::Vector2Int& p_chunkPosition) const;

		static spk::Vector2Int convertWorldToChunkPosition(const spk::Vector2Int& p_position);
	};
}