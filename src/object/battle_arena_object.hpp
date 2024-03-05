#pragma once

#include "sparkle.hpp"

#include "miscellaneous/texture_atlas.hpp"

class BattleArenaObject : public spk::GameObject
{
public:
	struct TileObject : public spk::GameObject
	{
		enum class Type
		{
			Standard,
			Ally,
			Enemy,
			Player,
			Border,
			Obstacle,
			AllyStartingPosition,
			EnemyStartingPosition
		};

		enum class Status
		{
			None,
			Movement,
			Range,
			EffectArea
		};

		spk::GameObject _statusObject;
		spk::MeshRenderer* _statusRenderer;
		spk::GameObject _typeObject;
		spk::MeshRenderer* _typeRenderer;

		spk::StateMachine<Type> _type;
		spk::StateMachine<Status> _status;

		TileObject() :
			spk::GameObject("TileObject"),
			_statusObject("StatusObject", this),
			_statusRenderer(_statusObject.addComponent<spk::MeshRenderer>("Renderer")),
			_typeObject("TypeObject", this),
			_typeRenderer(_typeObject.addComponent<spk::MeshRenderer>("Renderer"))
		{
			setActivationCallback([&](){
				 
				});

			_typeObject.transform().translation = spk::Vector3(0, 0, 2.0f);
			_statusObject.transform().translation = spk::Vector3(0, 0, 2.0f);

			_statusRenderer->setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("BattleUISpriteSheet"));
			_typeRenderer->setSpriteSheet(TextureAtlas::instance()->as<spk::SpriteSheet>("BattleUISpriteSheet"));

			_type.addState(Type::Standard, spk::StateMachine<Type>::Action([&](){_typeRenderer->deactivate();}, nullptr, [&](){_typeRenderer->activate();}));
			_type.addState(Type::Obstacle, spk::StateMachine<Type>::Action([&](){_typeRenderer->deactivate();}, nullptr, [&](){_typeRenderer->activate();}));
			_type.addState(Type::Border, spk::StateMachine<Type>::Action([&](){_typeRenderer->setSprite(spk::Vector2Int(7, 0));}, nullptr, nullptr));
			_type.addState(Type::Ally, spk::StateMachine<Type>::Action([&](){_typeRenderer->setSprite(spk::Vector2Int(1, 0));}, nullptr, nullptr));
			_type.addState(Type::Enemy, spk::StateMachine<Type>::Action([&](){_typeRenderer->setSprite(spk::Vector2Int(2, 0));}, nullptr, nullptr));
			_type.addState(Type::Player, spk::StateMachine<Type>::Action([&](){_typeRenderer->setSprite(spk::Vector2Int(0, 0));}, nullptr, nullptr));
			_type.addState(Type::AllyStartingPosition, spk::StateMachine<Type>::Action([&](){_typeRenderer->setSprite(spk::Vector2Int(3, 0));}, nullptr, nullptr));
			_type.addState(Type::EnemyStartingPosition, spk::StateMachine<Type>::Action([&](){_typeRenderer->setSprite(spk::Vector2Int(4, 0));}, nullptr, nullptr));

			_status.addState(Status::None, spk::StateMachine<Status>::Action([&](){_statusRenderer->deactivate();}, nullptr, [&](){_statusRenderer->activate();}));
			_status.addState(Status::Movement, spk::StateMachine<Status>::Action([&](){_statusRenderer->setSprite(spk::Vector2Int(0, 1));}, nullptr, nullptr));
			_status.addState(Status::Range, spk::StateMachine<Status>::Action([&](){_statusRenderer->setSprite(spk::Vector2Int(1, 1));}, nullptr, nullptr));
			_status.addState(Status::EffectArea, spk::StateMachine<Status>::Action([&](){_statusRenderer->setSprite(spk::Vector2Int(2, 1));}, nullptr, nullptr));

			_type.enterState(Type::Standard);
			_status.enterState(Status::None);
		}

		void setStatus(const Status& p_status)
		{
			_status.enterState(p_status);
		}
		Status status() const
		{
			return (_status.state());
		}

		void setType(const Type& p_type)
		{
			_type.enterState(p_type);
		}
		Type type() const
		{
			return (_type.state());
		}
	};

private:
	using Tile = spk::Pool<TileObject>::Object;

	static inline spk::RandomGenerator<int> _randomGenerator = spk::RandomGenerator<int>(146257938);
	spk::Pool<TileObject> _tilePool;
	std::map<spk::Vector2Int, Tile> _tiles;
	std::vector<spk::Vector2Int> _battleTiles;

public:
	BattleArenaObject(const std::string& p_name) :
		spk::GameObject(p_name)
	{
		_tilePool.editAllocator([&]() -> BattleArenaObject::TileObject * {
				BattleArenaObject::TileObject *result = new BattleArenaObject::TileObject();
				addChild(result);
				result->deactivate();
				return result;
			});

		setActivationCallback([&](){
				for (auto& [key, element] : _tiles)
				{
					element->activate();
				}
			});

		setDeactivationCallback([&](){
			for (auto& [key, element] : _tiles) 
			{
				element->deactivate();
			}
			_battleTiles.clear();
			_tiles.clear();
		});
	}

	void addBattleTile(const spk::Vector2Int& p_position, TileObject::Type p_type)
	{
		Tile newTile = _tilePool.obtain();
		newTile->setType(p_type);
		newTile->transform().translation = spk::Vector3(p_position.x, p_position.y, 0);
		_tiles[p_position] = std::move(newTile);
		_battleTiles.push_back(p_position);
	}

	void addBorderTile(const spk::Vector2Int& p_position)
	{
		Tile newTile = _tilePool.obtain();
		newTile->setType(TileObject::Type::Border);
		newTile->transform().translation = spk::Vector3(p_position.x, p_position.y, 0);
		_tiles[p_position] = std::move(newTile);
	}

	void generateStartingPosition(const TileObject::Type& p_type, size_t nbTile)
	{
		size_t nbStartingPlace = 0;
		while (nbStartingPlace < nbTile)
		{
			_randomGenerator.configureRange(0, _battleTiles.size() - 1);
			int index = _randomGenerator.sample();

			const spk::Vector2Int& position = _battleTiles.at(index);
			auto& tmpTile = _tiles[position];
			if (tmpTile->type() == TileObject::Type::Standard)
			{
				tmpTile->setType(p_type);
				nbStartingPlace++;
			}
		}
	}

	void setStatus(const spk::Vector2Int& p_position, const TileObject::Status& p_state)
	{
		if (_tiles.contains(p_position) == false)
			return;

		_tiles[p_position]->setStatus(p_state);
	}
};