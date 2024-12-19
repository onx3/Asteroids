#pragma once

#include "GameComponent.h"
#include "DropManager.h"

class DropComponent : public GameComponent
{
public:
	DropComponent(GameObject * pOwner);

	void NotifyDropManagerOnDeath(const sf::Vector2f & position);

	void SetDropTable(std::vector<std::pair<EDrops, float>> dropTable);

	void Update() override;

private:
	bool RandomChance(float probability);

	std::vector<std::pair<EDrops, float>> mDropTable;
};