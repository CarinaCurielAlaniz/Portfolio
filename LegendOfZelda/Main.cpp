//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

int main(int argc, char** argv)
{
	Game mario;
	if (mario.Initialize())
	{
		mario.ReadFile("Assets/Level1.txt");
		mario.RunLoop();
		mario.Shutdown();
	}
	return 0;
}
