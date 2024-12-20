#include "PlayerManager.h"
#include "SpriteComponent.h"
#include "ControlledMovementComponent.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "ExplosionComponent.h"
#include <cassert>

PlayerManager::PlayerManager(GameManager * pGameManager)
    : BaseManager(pGameManager)
    , mSoundPlayed(false)
{
    InitPlayer();

    // Sound
    {
        assert(mLoseLifeSoundBuffer.loadFromFile("Audio/LoseLifeSound.wav"));
        mLoseLifeSound.setBuffer(mLoseLifeSoundBuffer);
        mLoseLifeSound.setVolume(100.f);
        
        assert(mDeathSoundBuffer.loadFromFile("Audio/Death.flac"));
        mDeathSound.setBuffer(mDeathSoundBuffer);
        mDeathSound.setVolume(50.f);
    }
}

//------------------------------------------------------------------------------------------------------------------------

PlayerManager::~PlayerManager()
{
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::InitPlayer()
{
    auto * pPlayer = mpGameManager->CreateNewGameObject(ETeam::Friendly, mpGameManager->GetRootGameObject());
    mPlayerObjects.push_back(pPlayer);

    // Sprite Component
    {
        sf::Vector2u windowSize = mpGameManager->mpWindow->getSize();
        sf::Vector2f centerPosition(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f);

        auto pSpriteComponent = pPlayer->GetComponent<SpriteComponent>().lock();
        if (pSpriteComponent)
        {
            std::string file = "Art/player.png";
            auto scale = sf::Vector2f(.5f, .5f);
            pSpriteComponent->SetSprite(file, scale);
            pSpriteComponent->SetPosition(centerPosition);
        }
    }

    // Controlled Movement Component
    {
        auto pMovementComponent = pPlayer->GetComponent<ControlledMovementComponent>().lock();
        if (!pMovementComponent)
        {
            auto pMovementComponent = std::make_shared<ControlledMovementComponent>(pPlayer);
            pPlayer->AddComponent(pMovementComponent);
        }
    }

    // Projectile Component
    {
        auto pProjectileComponent = pPlayer->GetComponent<ProjectileComponent>().lock();
        if (!pProjectileComponent)
        {
            pPlayer->AddComponent(std::make_shared<ProjectileComponent>(pPlayer));
        }
    }

    // Health Component
    {
        auto pHealthComponent = pPlayer->GetComponent<HealthComponent>().lock();
        if (!pHealthComponent)
        {
            pPlayer->AddComponent(std::make_shared<HealthComponent>(pPlayer, 102220, 102220, 3, 3, 2.f));
        }
    }

    // Collision Component
    {
        auto pCollisionComponent = pPlayer->GetComponent<CollisionComponent>().lock();
        if (!pCollisionComponent)
        {
            pPlayer->AddComponent(std::make_shared<CollisionComponent>(pPlayer, pPlayer->GetSize()));
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::Update()
{
    CleanUpDeadPlayers();

    if (mPlayerObjects.empty())
    {
        mpGameManager->EndGame();
    }
    else
    {
        for (auto * pPlayer : mPlayerObjects)
        {
            // Destroy the player after the explosion animation finishes
            auto explosionComp = pPlayer->GetComponent<ExplosionComponent>().lock();
            if (explosionComp && explosionComp->IsAnimationFinished())
            {
                pPlayer->Destroy();
                return;
            }

            auto pHealthComp = pPlayer->GetComponent<HealthComponent>().lock();

            if (pHealthComp)
            {
                // Set the callbacks
                pHealthComp->SetLifeLostCallback([this, pPlayer]() {
                    OnPlayerLostLife(pPlayer);
                    });
                pHealthComp->SetDeathCallBack([this, pPlayer]() {
                    OnPlayerDeath(pPlayer);
                    });
            }

            if (pHealthComp && pHealthComp->GetLives() == 1)
            {
                auto pSpriteComponent = pPlayer->GetComponent<SpriteComponent>().lock();
                if (pSpriteComponent)
                {
                    pSpriteComponent->SetSprite("Art/playerDamaged.png", pSpriteComponent->GetSprite().getScale());
                }
            }
        }

        if (mLoseLifeSound.getStatus() == sf::Sound::Stopped)
        {
            mSoundPlayed = false;
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::OnPlayerLostLife(GameObject * pPlayer)
{
    if (!mSoundPlayed)
    {
        mLoseLifeSound.play();
    }
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::OnPlayerDeath(GameObject * pPlayer)
{
    if (!mSoundPlayed)
    {
        mDeathSound.play();
        mSoundPlayed = true;
    }

    // Add the explosion animation here
    if (!pPlayer->GetComponent<ExplosionComponent>().lock())
    {
        auto explosionComp = std::make_shared<ExplosionComponent>(
            pPlayer, "Art/explosion.png", 32, 32, 7, 0.1f);
        pPlayer->AddComponent(explosionComp);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::CleanUpDeadPlayers()
{
    auto newEnd = std::remove_if(mPlayerObjects.begin(), mPlayerObjects.end(),
        [](GameObject * obj)
        {
            return obj->IsDestroyed();
        });

    mPlayerObjects.erase(newEnd, mPlayerObjects.end());
}

//------------------------------------------------------------------------------------------------------------------------

const std::vector<GameObject *> & PlayerManager::GetPlayers() const
{
    return mPlayerObjects;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------