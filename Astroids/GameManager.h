#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "EnemyAIManager.h"
#include "GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Update();
	void Render();

	sf::RenderWindow * mpWindow;
	sf::Event mEvent;

private:
	void PollEvents();

	void InitEnemies();
	void InitPlayer();
	void InitWindow();

	EnemyAIManager mEnemyManager;

	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;

	GameObject mPlayer;
};

