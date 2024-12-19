#include "DropManager.h"
#include "GameObject.h"
#include "GameManager.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "RandomMovementComponent.h"
#include "DropMovementComponent.h"

DropManager::DropManager(GameManager * pGameManager)
    : BaseManager(pGameManager)
    , mDropObjects()
{
}

//------------------------------------------------------------------------------------------------------------------------

DropManager::~DropManager()
{
}

//------------------------------------------------------------------------------------------------------------------------

void DropManager::Update()
{
}

//------------------------------------------------------------------------------------------------------------------------

void DropManager::SpawnDrop(const EDrops dropType, const sf::Vector2f & position)
{
    GameObject * pDropObject = nullptr;

    switch (dropType)
    {
        case EDrops::Health:
        {
            pDropObject = mpGameManager->CreateNewGameObject(ETeam::Neutral, mpGameManager->GetRootGameObject());

            auto pSpriteComp = pDropObject->GetComponent<SpriteComponent>().lock();
            pSpriteComp->SetSprite(GetSpriteFile(dropType));
            SetUpDropObject(pDropObject);
            break;
        }
        case EDrops::Nuke:
        {
            pDropObject = mpGameManager->CreateNewGameObject(ETeam::Neutral, mpGameManager->GetRootGameObject());

            auto pSpriteComp = pDropObject->GetComponent<SpriteComponent>().lock();
            pSpriteComp->SetSprite(GetSpriteFile(dropType));
            SetUpDropObject(pDropObject);
            break;
        }
        default:
        {
            return;
        }
    }

    mDropObjects.push_back(pDropObject);
    if (pDropObject)
    {
        pDropObject->SetPosition(position);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void DropManager::SetUpDropObject(GameObject * pDrop)
{
    // Collision Component
    {
        auto pCollisionComp = std::make_shared<CollisionComponent>(pDrop, pDrop->GetSize());
        pDrop->AddComponent<CollisionComponent>(pCollisionComp);
    }
    // Movement Component
    {
        auto pMovementComp = std::make_shared<DropMovementComponent>(pDrop);
        pDrop->AddComponent<DropMovementComponent>(pMovementComp);
    }
    
}

//------------------------------------------------------------------------------------------------------------------------

std::string DropManager::GetSpriteFile(EDrops dropType)
{
    switch (dropType)
    {
        case EDrops::Health:
        {
            return "Art/life.png";
        }
        case EDrops::Nuke:
        {
            return "Art/bomb64.png";
        }
        default:
        {
            return "Art/laserRedShot.png";
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------