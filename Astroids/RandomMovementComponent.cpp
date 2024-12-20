#include "RandomMovementComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "GameManager.h"
#include <SFML/Graphics.hpp>
#include <random>

//------------------------------------------------------------------------------------------------------------------------

RandomMovementComponent::RandomMovementComponent(GameObject * pOwner)
    : GameComponent(pOwner)
    , mVelocity(300.f)
    , mIsEntering(true)
    , mSkipBoundaryCheck(true)
    , mHasEnteredScreen(false)
    , mName("RandomMovementComponent")
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<float> randomDirection(-1.0f, 1.0f);
    static std::uniform_int_distribution<int> randomEdge(0, 3);

    auto pSpriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();
    if (pSpriteComponent)
    {
        // Generate a random direction vector for later movement
        sf::Vector2f randomVector(randomDirection(rng), randomDirection(rng));
        float magnitude = std::sqrt(randomVector.x * randomVector.x + randomVector.y * randomVector.y);
        if (magnitude != 0)
        {
            mDirection = randomVector / magnitude; // Normalize for random movement
        }

        // Initialize intro movement
        mStartPosition = pSpriteComponent->GetPosition(); // The off-screen spawn position

        // Set target position to a random edge of the screen
        sf::Vector2u windowSize = GetGameObject().GetGameManager().mpWindow->getSize();
        int edge = randomEdge(rng);

        switch (edge)
        {
            case 0: // Top
                mTargetPosition = { static_cast<float>(rand() % windowSize.x), 100.f };
                break;
            case 1: // Bottom
                mTargetPosition = { static_cast<float>(rand() % windowSize.x), static_cast<float>(windowSize.y - 100.f) };
                break;
            case 2: // Left
                mTargetPosition = { 100.f, static_cast<float>(rand() % windowSize.y) };
                break;
            case 3: // Right
                mTargetPosition = { static_cast<float>(windowSize.x - 100.f), static_cast<float>(rand() % windowSize.y) };
                break;
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

RandomMovementComponent::~RandomMovementComponent()
{
}

//------------------------------------------------------------------------------------------------------------------------

void RandomMovementComponent::Update()
{
    if (mpOwner->IsActive())
    {
        auto spriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();
        if (spriteComponent)
        {
            sf::Vector2f position = spriteComponent->GetPosition();
            sf::Vector2u windowSize = GetGameObject().GetGameManager().mpWindow->getSize();
            sf::Vector2f size(spriteComponent->GetWidth(), spriteComponent->GetHeight());

            // Intro phase movement
            if (mIsEntering)
            {
                sf::Vector2f direction = mTargetPosition - position;
                float distance = std::hypot(direction.x, direction.y);

                if (distance > 1.0f)
                {
                    direction /= distance;
                    position += direction * mVelocity * GetGameObject().GetDeltaTime();
                }
                else
                {
                    position = mTargetPosition;
                    mIsEntering = false;
                    mHasEnteredScreen = true;
                }

                spriteComponent->SetPosition(position);
                return;
            }
            position += mDirection * mVelocity * GetGameObject().GetDeltaTime();

            // Check for collisions with window bounds only after fully entering
            if (mHasEnteredScreen)
            {
                if (position.x < 0 || position.x + size.x > windowSize.x)
                {
                    mDirection.x *= -1;
                    position.x = std::clamp(position.x, 0.f, windowSize.x - size.x);
                }
                if (position.y < 0 || position.y + size.y > windowSize.y)
                {
                    mDirection.y *= -1;
                    position.y = std::clamp(position.y, 0.f, windowSize.y - size.y);
                }
            }
            spriteComponent->SetPosition(position);
        }
    }
}