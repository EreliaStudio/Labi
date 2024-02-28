#pragma once

#include "sparkle.hpp"

class TilemapGenerator
{
public:
	struct Area
	{
		spk::Vector2Int anchor;
		spk::Vector2Int size;
	};
	using OutputFormat = std::vector<std::vector<int>>;
	using AreaMap = std::map<spk::Vector2Int, bool>;
	using AreaList = std::vector<Area>;
	using ChunkList = std::vector<spk::Vector2Int>;

	enum class Type
	{
		Cave
	};

private:

	intmax_t _tilemapSeed = 123456789;
	intmax_t _areaMapSeed = 123456789;
	spk::Vector2Int _areaChunkSize = 16;
	spk::Vector2Int _minAreaSize = 1;
	spk::Vector2Int _maxAreaSize = 4;

public:
	TilemapGenerator();
	TilemapGenerator(const intmax_t& p_tilemapSeed, const intmax_t& p_areaMapSeed);

	void configureSeed(const intmax_t& p_tilemapSeed, const intmax_t& p_areaMapSeed);
	void configureAreaChunkSize(const spk::Vector2Int& p_areaChunkSize);
	void configureAreaSize(const spk::Vector2Int& p_minimalSize, const spk::Vector2Int& p_maximalSize);

	OutputFormat generate(const Type& p_type, const spk::Vector2Int& p_nbChunk);
};