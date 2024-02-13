#pragma once

#include "math/spk_vector2.hpp"

namespace spk
{
	struct Node
	{
		using IndexType = int8_t;
		
		spk::Vector2Int sprite;
		spk::Vector2Int spriteOffset;
		bool isAutotiled = false;

		Node();
		Node(spk::Vector2Int p_sprite, spk::Vector2Int p_spriteOffset, bool p_isAutotiled);
	};
}