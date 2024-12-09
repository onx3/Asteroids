#pragma once

#include <SFML/Graphics.hpp>
#include "GameComponent.h"

class SpriteComponent : public GameComponent
{
public:
	SpriteComponent(GameObject * pOwner);
	~SpriteComponent();

	void SetSprite(const std::string & file);
	sf::Sprite GetSprite();

	sf::Vector2f GetPosition() const;
	void SetPosition(const sf::Vector2f & position);

	float GetWidth() const;
	float GetHeight() const;

	void Move(const sf::Vector2f & offset);
	void Move(float x, float y);

	void SetRotation(float angle);
	void SetOriginToCenter();

	void Update() override;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;
};

