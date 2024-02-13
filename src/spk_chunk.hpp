#pragma once

#include "game_engine/spk_game_object.hpp"
#include "graphics/texture/spk_sprite_sheet.hpp"
#include "game_engine/component/spk_mesh_renderer_component.hpp"
#include "spk_node.hpp"

namespace spk
{
	class Tilemap;

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
			InternalComponent* _neightbourChunks[3][3];
			spk::Mesh _mesh;

			void _insertSingleTileToMesh(size_t p_x, size_t p_y, size_t p_z, const Node& p_node);
			void _insertAutotileToMesh(size_t p_x, size_t p_y, size_t p_z, const Node& p_node);

			void _insertTileToMesh(size_t p_x, size_t p_y, size_t p_z, const Node &p_node);

			void _setNeighbourChunks(const Tilemap* p_tilemap);
			void _bake(const std::map<Node::IndexType, Node>& p_nodeMap);

			void _onRender();
			void _onUpdate();

		public:
			InternalComponent(const std::string& p_name);

			void setContent(const spk::Vector2Int& p_position, size_t p_height, int8_t p_value);
			int8_t content(const spk::Vector2Int& p_position, size_t p_height);

			void launchBake(const Tilemap* p_tilemap);
		};

		spk::MeshRenderer* _meshRenderer;
		InternalComponent* _internalComponent;
		
	public:
		Chunk(const spk::Vector2Int& p_position);

		void setContent(const spk::Vector2Int& p_position, size_t p_height, int8_t p_value);
		int8_t content(const spk::Vector2Int& p_position, size_t p_height);

		void launchBake(const Tilemap* p_tilemap);

		static spk::Vector2Int convertAbsoluteToRelativePosition(const spk::Vector2Int& p_position);
	};
}