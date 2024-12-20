#pragma once

#include "GameComponent.h"

class ControlledMovementComponent : public GameComponent
{
public:
	ControlledMovementComponent(GameObject * pOwner);
	ControlledMovementComponent(GameObject * pOwner, float veloX, float veloY);

	~ControlledMovementComponent();

	void Update() override;

	std::string & GetClassName() override;

	void SetVelocityX(float velo);
	void SetVelocityY(float velo);
private:

	sf::Vector2f mVelocity;
	float mVelocityX;
	float mVelocityY;
	float mAcceleration;
	float mDeceleration;
	float mMaxSpeed;
	std::string mName;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------