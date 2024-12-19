#pragma once
#include "BaseManager.h"
#include <SFML/graphics.hpp>
#include "GameObject.h"

enum class EDrops
{
	Health,
	Nuke
};

class DropManager : public BaseManager
{
public:
	DropManager(GameManager * pGameManager);
	~DropManager();

	void Update();
	void SpawnDrop(const EDrops dropType, const sf::Vector2f & position);

private:
	std::string GetSpriteFile(EDrops dropType);
	void SetUpDropObject(GameObject * pDrop);

	std::vector<GameObject *> mDropObjects;
};