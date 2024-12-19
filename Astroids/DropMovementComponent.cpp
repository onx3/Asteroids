#include "DropMovementComponent.h"
#include <cmath>
#include <cstdlib>

DropMovementComponent::DropMovementComponent(GameObject * pOwner)
	: GameComponent(pOwner)
	, mVelocity(150.f)
	, mDirection(0.f, 0.f)
{
    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;

    mDirection.x = std::cos(angle);
    mDirection.y = std::sin(angle);

    float magnitude = std::sqrt(mDirection.x * mDirection.x + mDirection.y * mDirection.y);
    if (magnitude > 0.f)
    {
        mDirection.x /= magnitude;
        mDirection.y /= magnitude;
    }
}

//------------------------------------------------------------------------------------------------------------------------

DropMovementComponent::~DropMovementComponent()
{

}

//------------------------------------------------------------------------------------------------------------------------

void DropMovementComponent::Update()
{
    if (!mpOwner) return;

    sf::Vector2f movement = mDirection * mVelocity * mpOwner->GetDeltaTime();

    sf::Vector2f currentPosition = mpOwner->GetPosition();
    mpOwner->SetPosition(currentPosition + movement);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------