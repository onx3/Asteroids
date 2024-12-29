#pragma once
#include "BaseManager.h"
#include "GameManager.h"

class PlayerManager : public BaseManager
{
public:
    PlayerManager(GameManager * pGameManager);
    ~PlayerManager();

    void InitPlayer();

    void Update() override;

    void OnGameEnd() override;

    void OnPlayerLostLife(GameObject * pPlayer);

    void OnPlayerDeath(GameObject * pPlayer);

    const std::vector<GameObject *> & GetPlayers() const;

private:
    void CleanUpDeadPlayers();
    std::vector<GameObject *> mPlayerObjects;

    // Audio
    sf::SoundBuffer mLoseLifeSoundBuffer;
    sf::SoundBuffer mDeathSoundBuffer;
    sf::Sound mLoseLifeSound;
    sf::Sound mDeathSound;
    bool mSoundPlayed;
};
