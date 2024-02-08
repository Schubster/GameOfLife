#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <list>
#include <iostream>
#include <algorithm>


//game engine

class Game
{

private:

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Vector2i mouseWindowPos;
	std::vector<sf::RectangleShape> rects;
	std::vector<sf::Vector2i> nextCords;
	float recSize = 10;
	bool gameStart = false;
	int framesPerGen = 400;
	int currentFrame = 0;

	//game obj
	sf::RectangleShape rectangle;

	void initVars();
	void initWindow();
	void initRec();


public:

	//constructors /destructors
	Game();
	virtual ~Game();

	//accessors
	const bool running() const;



	void update();
	void render();
	void getMouseWindowPos();
	void pollEvents();
	sf::RectangleShape newRec(int x, int y);
	int roundDown(int numToRound);
	bool recExists(int x, int y);
	int checkNeighbor(int x, int y);
	std::vector<sf::RectangleShape> checkDeadCell();
	void updateGen();

};

