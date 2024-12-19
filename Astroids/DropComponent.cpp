#include "DropComponent.h"

DropComponent::DropComponent(GameObject * pOwner)
	: GameComponent(pOwner)
{
}

//------------------------------------------------------------------------------------------------------------------------

void DropComponent::NotifyDropManagerOnDeath(const sf::Vector2f & position)
{
    if (!mpOwner)
    {
        return;
    }

    auto * pDropManager = mpOwner->GetGameManager().GetManager<DropManager>();
    if (!pDropManager)
    {
        return;
    }

    for (const auto & drop : mDropTable)
    {
        if (RandomChance(drop.second))
        {
            pDropManager->SpawnDrop(drop.first, position);
            break;
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void DropComponent::SetDropTable(std::vector<std::pair<EDrops, float>> dropTable)
{
    mDropTable = dropTable;
}

//------------------------------------------------------------------------------------------------------------------------

void DropComponent::Update()
{
}

//------------------------------------------------------------------------------------------------------------------------

bool DropComponent::RandomChance(float probability)
{
    auto chance = (float(rand()) / RAND_MAX) < probability;
    return chance;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------