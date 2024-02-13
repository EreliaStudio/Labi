#include "spk_chunk.hpp"
#include "spk_tilemap.hpp"

namespace spk
{
	void Chunk::InternalComponent::_insertSingleTileToMesh(size_t p_x, size_t p_y, size_t p_z, const Node &p_node)
	{
		unsigned int indexes[6] = {0, 1, 2, 2, 1, 3};
		unsigned int nbVertices = _mesh.points().size();

		_mesh.addPoint(spk::Vector3(p_x + 1 - 0.5f, p_y + 1 - 0.5f, p_z));
		_mesh.addPoint(spk::Vector3(p_x + 0 - 0.5f, p_y + 1 - 0.5f, p_z));
		_mesh.addPoint(spk::Vector3(p_x + 1 - 0.5f, p_y + 0 - 0.5f, p_z));
		_mesh.addPoint(spk::Vector3(p_x + 0 - 0.5f, p_y + 0 - 0.5f, p_z));

		_mesh.addUVs(p_node.sprite + spk::Vector2Int(1, 0));
		_mesh.addUVs(p_node.sprite + spk::Vector2Int(0, 0));
		_mesh.addUVs(p_node.sprite + spk::Vector2Int(1, 1)); 
		_mesh.addUVs(p_node.sprite + spk::Vector2Int(0, 1));

		for (size_t i = 0; i < 6; i++)
		{
			_mesh.addIndex(indexes[i] + nbVertices, indexes[i] + nbVertices);
		}
	}

	void Chunk::InternalComponent::_setNeighbourChunks(const Tilemap* p_tilemap)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int i = 0; i < 3; i++)
			{
				
			}
		}
	}

	spk::Vector2Int _computeAutotileSubdivision(size_t p_x, size_t p_y, size_t p_z, size_t p_face)
	{
		static const spk::Vector2Int neightbours[4][3] = {
			{
				spk::Vector2Int(-1, 0),
				spk::Vector2Int(-1, -1),
				spk::Vector2Int(0, -1),
			},
			{
				spk::Vector2Int(1, 0),
				spk::Vector2Int(1, -1),
				spk::Vector2Int(0, -1),
			},
			{
				spk::Vector2Int(-1, 0),
				spk::Vector2Int(-1, 1),
				spk::Vector2Int(0, 1),
			},
			{
				spk::Vector2Int(1, 0),
				spk::Vector2Int(1, 1),
				spk::Vector2Int(0, 1),
			}
		};

		int neightbourA = 0;
		int neightbourB = 0;
		int neightbourC = 0;

		static const spk::Vector2Int results[4][2][2][2] = {
			{
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
				},
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					}
				}
			},
			{
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
				},
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					}
				}
			},
			{
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
				},
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					}
				}
			},
			{
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
				},
				{
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					},
					{
						spk::Vector2Int(1, 1),
						spk::Vector2Int(1, 1)
					}
				}
			}
		};

		return (results[p_face][neightbourA][neightbourB][neightbourC]);
	}

	void Chunk::InternalComponent::_insertAutotileToMesh(size_t p_x, size_t p_y, size_t p_z, const Node &p_node)
	{
		unsigned int indexes[6] = {0, 1, 2, 2, 1, 3};
		spk::Vector3 offsets[4] = {
			spk::Vector3(0.0f, 0.0f, 0.0f),
			spk::Vector3(0.5f, 0.0f, 0.0f),
			spk::Vector3(0.0f, 0.5f, 0.0f),
			spk::Vector3(0.5f, 0.5f, 0.0f)};

		for (size_t i = 0; i < 4; i++)
		{
			unsigned int nbVertices = _mesh.points().size();

			_mesh.addPoint(spk::Vector3(p_x + 0.0f, p_y + 0.0f, p_z) + offsets[i]);
			_mesh.addPoint(spk::Vector3(p_x - 0.5f, p_y + 0.0f, p_z) + offsets[i]);
			_mesh.addPoint(spk::Vector3(p_x + 0.0f, p_y - 0.5f, p_z) + offsets[i]);
			_mesh.addPoint(spk::Vector3(p_x - 0.5f, p_y - 0.5f, p_z) + offsets[i]);

			spk::Vector2Int autotileOffset = p_node.sprite + _computeAutotileSubdivision(p_x, p_y, p_z, i);
			_mesh.addUVs(autotileOffset + spk::Vector2Int(1, 0));
			_mesh.addUVs(autotileOffset + spk::Vector2Int(0, 0));
			_mesh.addUVs(autotileOffset + spk::Vector2Int(1, 1)); 
			_mesh.addUVs(autotileOffset + spk::Vector2Int(0, 1));

			for (size_t i = 0; i < 6; i++)
			{
				_mesh.addIndex(indexes[i] + nbVertices, indexes[i] + nbVertices);
			}
		}
	}

	void Chunk::InternalComponent::_insertTileToMesh(size_t p_x, size_t p_y, size_t p_z, const Node &p_node)
	{
		if (p_node.isAutotiled == true)
		{
			_insertAutotileToMesh(p_x, p_y, p_z, p_node);
		}
		else
		{
			_insertSingleTileToMesh(p_x, p_y, p_z, p_node);
		}
	}

	void Chunk::InternalComponent::_bake(const std::map<Node::IndexType, Node>& p_nodeMap)
	{
		_mesh.clear();

		for (size_t i = 0; i < Size; i++)
		{
			for (size_t j = 0; j < Size; j++)
			{
				for (size_t h = 0; h < Height; h++)
				{
					int8_t nodeIndex = _data[i][j][h];
					if (nodeIndex != -1 && p_nodeMap.contains(nodeIndex) == true)
						_insertTileToMesh(i, j, h, p_nodeMap.at(nodeIndex));
				} 
			}
		}
	}

	void Chunk::InternalComponent::_onRender()
	{
	}

	void Chunk::InternalComponent::_onUpdate()
	{
	}

	Chunk::InternalComponent::InternalComponent(const std::string &p_name) :
		spk::GameComponent(p_name),
		_ownerMeshRenderer(owner()->getComponent<spk::MeshRenderer>()),
		_neightbourChunks{
			{nullptr, nullptr, nullptr},
			{nullptr, this, nullptr},
			{nullptr, nullptr, nullptr},
		}
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
					if (k == 0)
					{
						_data[i][j][k] = ((i == 0 || j == 0) ? 0 : 1);
					}
					else
					{ 
						_data[i][j][k] = -1;
					}
				}
			}
		}
	}

	void Chunk::InternalComponent::setContent(const spk::Vector2Int &p_position, size_t p_height, int8_t p_value)
	{
		if (p_position.x >= 0 && p_position.x < Size &&
			p_position.y >= 0 && p_position.y < Size &&
			p_height >= 0 && p_height < Height)
		{
			_data[p_position.x][p_position.y][p_height] = p_value;
		}
	}

	int8_t Chunk::InternalComponent::content(const spk::Vector2Int &p_position, size_t p_height)
	{
		if (p_position.x >= 0 && p_position.x < Size &&
			p_position.y >= 0 && p_position.y < Size &&
			p_height >= 0 && p_height < Height)
		{
			return (_data[p_position.x][p_position.y][p_height]);
		}
		return (-1);
	}

	void Chunk::InternalComponent::launchBake(const Tilemap* p_tilemap)
	{
		_setNeighbourChunks(p_tilemap);
		_bake(p_tilemap->nodeMap());
		_mesh.validate();
	}

	Chunk::Chunk(const spk::Vector2Int &p_position) : spk::GameObject("Chunk [" + p_position.to_string() + "]")
	{
		if (SpriteSheet == nullptr)
			SpriteSheet = new spk::SpriteSheet("worldBackground.png", spk::Vector2Int(8, 6));

		transform().translation = spk::Vector3(p_position.x * static_cast<int>(Size), p_position.y * static_cast<float>(Size), 0);

		_meshRenderer = addComponent<spk::MeshRenderer>("MeshRenderer");
		_meshRenderer->setSpriteSheet(SpriteSheet);
		_meshRenderer->setSprite(spk::Vector2Int(0, 0));

		_internalComponent = addComponent<InternalComponent>("InternalComponent");
		deactivate();
	}

	void Chunk::setContent(const spk::Vector2Int &p_position, size_t p_height, int8_t p_value)
	{
		_internalComponent->setContent(p_position, p_height, p_value);
	}

	int8_t Chunk::content(const spk::Vector2Int &p_position, size_t p_height)
	{
		return (_internalComponent->content(p_position, p_height));
	}

	void Chunk::launchBake(const Tilemap* p_tilemap)
	{
		_internalComponent->launchBake(p_tilemap);
	}

	spk::Vector2Int Chunk::convertAbsoluteToRelativePosition(const spk::Vector2Int &p_position)
	{
		return (spk::Vector2Int::floor((p_position / static_cast<float>(Size))));
	}
}