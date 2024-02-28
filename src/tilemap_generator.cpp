#include "tilemap_generator.hpp"

TilemapGenerator::OutputFormat setupTilemap(const spk::Vector2Int& p_tilemapSize)
{
	TilemapGenerator::OutputFormat result;

	result.resize(p_tilemapSize.x);
	for (int i = 0; i < p_tilemapSize.x; i++)
	{
		result[i].resize(p_tilemapSize.y);
		for (int j = 0; j < p_tilemapSize.y; j++)
		{
			result[i][j] = -1;
		}
	}

	return (result);
}

void randomizeTilemap(TilemapGenerator::OutputFormat& p_tilemap, long long p_seed)
{
	spk::PseudoRandomGenerator<int> randomGenerator(p_seed);
	randomGenerator.configureRange(-1, 1);

	for (int i = 0; i < p_tilemap.size(); i++)
	{
		for (int j = 0; j < p_tilemap[i].size(); j++)
		{
			if (randomGenerator.sample(i, j) == 0)
				p_tilemap[i][j] = 0;
		}
	}
}

size_t countNeighbour(const TilemapGenerator::OutputFormat& p_tilemap, int p_x, int p_y)
{
    int result = 0;

    for (int ni = -1; ni <= 1; ni++)
    {
        for (int nj = -1; nj <= 1; nj++)
        {
            if (ni == 0 && nj == 0)
            {
                continue;
            }

            int neighborX = p_x + ni;
            int neighborY = p_y + nj;

			if (neighborX < 0 || neighborX >= p_tilemap.size() ||
				neighborY < 0 || neighborY >= p_tilemap[neighborX].size() ||
				p_tilemap[neighborX][neighborY] == 0)
			{	
            	result++;
			}
        }
    }

    return result;
}

void applyCellularAutomata(TilemapGenerator::OutputFormat& p_tilemap)
{
    TilemapGenerator::OutputFormat baseTilemap = p_tilemap;

    for (size_t i = 0; i < p_tilemap.size(); i++)
    {
        for (size_t j = 0; j < p_tilemap[i].size(); j++)
        {
            int wallNeighbors = countNeighbour(baseTilemap, i, j);

            if (baseTilemap[i][j] == 0 && wallNeighbors < 4)
            {
                p_tilemap[i][j] = -1;
            }
            else if (baseTilemap[i][j] == -1 && wallNeighbors > 5)
            {
                p_tilemap[i][j] = 0;
            }
        }
    }
}

void materializeArea(TilemapGenerator::OutputFormat& p_tilemap, const spk::Vector2Int& p_firstPoint, const spk::Vector2Int& p_secondPoint, size_t wallSize)
{
    for (int x = p_firstPoint.x; x <= p_secondPoint.x; x++)
    {
        for (int y = p_firstPoint.y; y <= p_secondPoint.y; y++)
        {
            if ((x - p_firstPoint.x < wallSize) ||
				(y - p_firstPoint.y < wallSize) ||
				(p_secondPoint.x - x < wallSize) ||
				(p_secondPoint.y - y < wallSize))
            {
                if (x >= 0 && x < p_tilemap.size() && y >= 0 && y < p_tilemap[0].size())
                {
                    p_tilemap[x][y] = 0;
                }
            }
        }
    }
}

TilemapGenerator::AreaMap setupAreaMap(const spk::Vector2Int& p_tilemapSize)
{
	TilemapGenerator::AreaMap result;

	for (size_t i = 0; i < p_tilemapSize.x; i++)
	{
		for (size_t j = 0; j < p_tilemapSize.x; j++)
		{
			result[spk::Vector2Int(i, j)] = false;
		}
	}

	return (result);
}

TilemapGenerator::ChunkList setupChunkList(const spk::Vector2Int& p_tilemapSize)
{
	TilemapGenerator::ChunkList result;

	for (size_t i = 0; i < p_tilemapSize.x; i++)
	{
		for (size_t j = 0; j < p_tilemapSize.x; j++)
		{
			result.push_back(spk::Vector2Int(i, j));
		}
	}

	return (result);
}

spk::Vector2Int computeAreaSize(const TilemapGenerator::AreaMap& p_areaMap, const spk::Vector2Int& p_start, const spk::Vector2Int& p_minimalSize, const spk::Vector2Int& p_maximalSize)
{
    spk::Vector2Int result = p_minimalSize;

    for (int width = result.x; width <= p_maximalSize.x; ++width)
    {
        for (int height = result.y; height <= p_maximalSize.y; ++height)
        {
            bool isValid = true;

            for (int x = 0; x < width && isValid; x++)
            {
                for (int y = 0; y < height && isValid; y++)
                {
                    spk::Vector2Int currentPos(p_start.x + x, p_start.y + y);
                    
                    if (p_areaMap.find(currentPos) == p_areaMap.end() || p_areaMap.at(currentPos))
                    {
                        isValid = false;
                        break;
                    }
                }
            }

            if (isValid == true)
            {
                if (width * height > result.x * result.y)
                {
                    result = spk::Vector2Int(width, height);
                }
            }
            else
            {
                break;
            }
        }
    }

    return result;
}

void createArea(TilemapGenerator::OutputFormat& p_tilemap, TilemapGenerator::AreaMap& p_areaMap, TilemapGenerator::ChunkList& p_chunkList, spk::Vector2Int p_start, const spk::Vector2Int& p_areaChunkSize, const spk::Vector2Int& p_minimalSize, const spk::Vector2Int& p_maximalSize)
{
	spk::Vector2Int size = computeAreaSize(p_areaMap, p_start, p_minimalSize, p_maximalSize);

	materializeArea(p_tilemap, p_start * p_areaChunkSize, (p_start + size) * p_areaChunkSize - 1, 1);

	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			spk::Vector2Int tmpPosition = p_start + spk::Vector2Int(i, j);
			p_areaMap[tmpPosition] = true;
			p_chunkList.erase(std::find(p_chunkList.begin(), p_chunkList.end(), tmpPosition));
		}
	}
}

void generateAreas(TilemapGenerator::OutputFormat& p_tilemap, const spk::Vector2Int& p_tilemapAreaSize, const spk::Vector2Int& p_areaChunkSize, const spk::Vector2Int& p_minimalSize, const spk::Vector2Int& p_maximalSize, long long p_seed)
{
	TilemapGenerator::AreaMap areaMap = setupAreaMap(p_tilemapAreaSize);
	TilemapGenerator::ChunkList chunkList = setupChunkList(p_tilemapAreaSize);

	spk::RandomGenerator<int> randomGenerator(p_seed);

	materializeArea(p_tilemap, 0, p_tilemapAreaSize * p_areaChunkSize - 1, 2);

	while (chunkList.empty() == false)
	{
		randomGenerator.configureRange(0, chunkList.size() - 1);
		
		size_t index = randomGenerator.sample();

		createArea(p_tilemap, areaMap, chunkList, chunkList[index], p_areaChunkSize, p_minimalSize, p_maximalSize);
	}
}

TilemapGenerator::TilemapGenerator()
{

}

TilemapGenerator::TilemapGenerator(const intmax_t& p_tilemapSeed, const intmax_t& p_areaMapSeed) : 
	_tilemapSeed(p_tilemapSeed),
	_areaMapSeed(p_areaMapSeed)
{

}

void TilemapGenerator::configureSeed(const intmax_t& p_tilemapSeed, const intmax_t& p_areaMapSeed)
{
	_tilemapSeed = p_tilemapSeed;
	_areaMapSeed = p_areaMapSeed;
}

void TilemapGenerator::configureAreaChunkSize(const spk::Vector2Int& p_areaChunkSize)
{	
	_areaChunkSize = p_areaChunkSize;
}

void TilemapGenerator::configureAreaSize(const spk::Vector2Int& p_minimalSize, const spk::Vector2Int& p_maximalSize)
{
	_minAreaSize = p_minimalSize;
	_maxAreaSize = p_maximalSize;
}

TilemapGenerator::OutputFormat TilemapGenerator::generate(const Type& p_type, const spk::Vector2Int& p_nbChunk)
{
	OutputFormat result = setupTilemap(p_nbChunk * _areaChunkSize);

	generateAreas(result, p_nbChunk, _areaChunkSize, _minAreaSize, _maxAreaSize, _areaMapSeed);

	//randomizeTilemap(result, _tilemapSeed);

	for (size_t i = 0; i < 3; i++)
		applyCellularAutomata(result);

	return (result);
}