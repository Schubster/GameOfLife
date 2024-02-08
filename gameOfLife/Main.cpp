#include <iostream>

#include "Game.h"


int main()
{

	Game game;

	


	//gane loop
	while(game.running())
	{
		
		game.update();

		game.render();

	}

	return 0;
}

