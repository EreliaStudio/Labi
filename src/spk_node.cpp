#include "spk_node.hpp"

namespace spk
{
	Node::Node() :
		sprite(0),
		spriteOffset(spk::Vector2Int(1, 0)),
		isAutotiled(false)
	{

	}

	Node::Node(spk::Vector2Int p_sprite, spk::Vector2Int p_spriteOffset, bool p_isAutotiled) :
		sprite(p_sprite),
		spriteOffset(p_spriteOffset),
		isAutotiled(p_isAutotiled)
	{

	}
}