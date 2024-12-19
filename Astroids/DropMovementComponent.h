#pragma once
#include "GameComponent.h"
#include "GameObject.h"


class DropMovementComponent : public GameComponent
{
public:
	DropMovementComponent(GameObject * pOwner);
	~DropMovementComponent();

	void Update() override;

private:
	float mVelocity;
	sf::Vector2f mDirection;
};