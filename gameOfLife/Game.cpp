#include "Game.h"


void Game::initVars()
{
	this->window = nullptr;
}

void Game::initWindow()
{

	this->videoMode.height = 600;
	this->videoMode.width = 800;
	
	this->window = new sf::RenderWindow(sf::VideoMode(this->videoMode), "Game Of Life", sf::Style::Titlebar | sf::Style::Close);
}

void Game::initRec()
{
	this->rectangle.setPosition(this->videoMode.width/2, this->videoMode.height/2);
	this->rectangle.setSize(sf::Vector2f(5.f, 5.f));
	this->rectangle.setFillColor(sf::Color::White);
	this->rectangle.setOutlineColor(sf::Color::Black);
	this->rectangle.setOutlineThickness(1.f);
}

Game::Game()
{
	this->initVars();
	this->initWindow();
	this->initRec();
}

Game::~Game()
{
	delete this->window;
}

const bool Game::running() const
{
	return this->window && this->window->isOpen();
}


//functions here


void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			if (this->ev.key.code == sf::Keyboard::Enter)
			{
				std::cout << "röven";
				this->currentFrame = 0;
				this->gameStart = !this->gameStart;
			}
				
			break;

		
		case sf::Event::MouseButtonPressed:
			this->mouseDown = true;
			break;

			case sf::Event::MouseButtonReleased:
				this->mouseDown = false;
				break;
		}
		if (this->mouseDown)
		{
			std::cout << "mouse pos:" << sf::Mouse::getPosition(*this->window).x << " " << sf::Mouse::getPosition(*this->window).y << "\n";

			sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
			int x = roundDown(mousePos.x);
			int y = roundDown(mousePos.y);
			if (!recExists(x, y))
			{
				this->rects.push_back(newRec(x, y));
				//checkNeighbor(this->rects.back());
				std::cout << "new rec";
			}
			else
			{
				std::cout << "rec exists";
			}
		}
	}
}
sf::RectangleShape Game::newRec(int x, int y)
{
	sf::RectangleShape rec;


	//rec.setPosition(roundDown(x, 20), roundDown(y, 20));
	rec.setPosition(x, y);
	this->nextCords.push_back(sf::Vector2i(x, y));
	rec.setSize(sf::Vector2f(recSize-1, recSize-1));
	rec.setFillColor(sf::Color::White);
	rec.setOutlineColor(sf::Color::Black);
	rec.setOutlineThickness(1.f);
	return rec;

}

void Game::update()
{
	this->pollEvents();

}

void Game::render()
{
	this->window->clear(sf::Color(0,0,0,0));
	this->window->draw(this->rectangle);
	if(this->currentFrame < this->framesPerGen)
		this->currentFrame++;
	
	if(this->gameStart && this->framesPerGen == this->currentFrame && !this->rects.empty())
	{
		updateGen();
		std::cout << "\nnewgen\n";
	}

	for (const auto& r : this->rects)
	{
		this->window->draw(r);
	}
	


		

	//DRAW GAME

	this->window->display();
}

void Game::getMouseWindowPos()
{
	this->mouseWindowPos = sf::Mouse::getPosition(*this->window);
}

int Game::roundDown(int numToRound)
{
	int multiple = this->recSize;
	return numToRound >= 0 ? (numToRound / multiple) * multiple : ((numToRound - multiple + 1) / multiple) * multiple;
}

bool Game::recExists(int x, int y)
{
	for(auto &rec : this->rects)
	{
		if (rec.getPosition() == sf::Vector2f(x, y))
			return true;
	}
	return false;
}

int Game::checkNeighbor(int x, int y)
{
	int ncount = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = 1; j >= -1; j--)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			int posX = j * this->recSize + x;
			int posY = i * this->recSize + y;
			if (recExists(posX, posY)) 
			{
				ncount++;
			}
		}
	}

	return ncount;
}

std::vector<sf::RectangleShape> Game::checkDeadCell()
{
	std::vector<sf::RectangleShape> newCells;
	int ncount = 0;
	for (const auto& rec : this->rects)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = 1; j >= -1; j--)
			{

				if (i == 0 && j == 0)
				{
					continue;
				}
				int x = j * this->recSize + rec.getPosition().x;
				int y = i * this->recSize + rec.getPosition().y;

				if (std::find(this->nextCords.begin(), this->nextCords.end(), sf::Vector2i(x, y)) != nextCords.end())
				{
					continue;
				}
				if (!recExists(x, y))
				{
					if (checkNeighbor(x, y) == 3)
					{
						newCells.push_back(newRec(x, y));
					}
				}
			}
			
		}
	}
	return newCells;
}

void Game::updateGen()
{
	this->currentFrame = 0;
	std::vector<sf::RectangleShape> deadList = checkDeadCell();
	std::vector<sf::RectangleShape> tempList = this->rects;
	for (int i = 0; i < tempList.size();i++)
	{
		int neighbors = checkNeighbor(tempList[i].getPosition().x, tempList[i].getPosition().y);
		if (!(neighbors == 2 || neighbors == 3))
		{
			tempList.erase(std::next(tempList.begin(), i));
			i--;
			std::cout << "\nremove: " << neighbors << "\n";
		}
			
	}
	this->rects = tempList;
	for (const auto& item : deadList)
	{
		this->rects.push_back(item);
	}
	this->nextCords.clear();
}


