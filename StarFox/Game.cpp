//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "SideBlock.h"
#include "Player.h"
#include "Block.h"
#include <iostream>
#include "PlayerMove.h"
#include <string>

Game::Game()
{
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
	mRenderer = new Renderer(this);
	bool check = mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!check)
	{
		SDL_Log("Unable to initialize renderer");
		return false;
	}

	mAudio = new AudioSystem();
	mAudio->PlaySound("Music.ogg", true);
	mSH = mAudio->PlaySound("ShipLoop.ogg", false);

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	std::vector<Actor*> copy = mActors;
	for (auto actor : copy)
	{
		actor->ProcessInput(state);
	}

	mAudio->ProcessInput(state);
}

void Game::UpdateGame()
{
	// Compute delta time
	Uint32 tickNow = SDL_GetTicks();
	// Wait until 16ms has elapsed since last frame
	while (tickNow - mTicksCount < 16.0f)
	{
		tickNow = SDL_GetTicks();
	}
	float diff = static_cast<float>(tickNow - mTicksCount);
	if (diff > 33.0f)
	{
		diff = 33.0f;
	}
	diff /= 1000.0f;

	// For 3D the games, force delta time to 16ms even if slower
	float deltaTime = diff;
	mTicksCount = SDL_GetTicks();

	mAudio->Update(deltaTime);

	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// Precache all the sounds (do not remove)
	mAudio->CacheAllSounds();
	mPlayer = new Player(this);
	Matrix4 projMatrix = Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f,
													   10000.0f);
	mRenderer->SetProjectionMatrix(projMatrix);
	Vector3 eye = Vector3::Zero;
	eye.x = -300;
	Vector3 target = Vector3::Zero;
	target.x = 20;

	Matrix4 viewMatrix = Matrix4::CreateLookAt(eye, target, Vector3::UnitZ);
	mRenderer->SetViewMatrix(viewMatrix);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mAudio;
	mRenderer->Shutdown();
	delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}
}
std::vector<class Block*>& Game::GetBlock()
{
	return mBlocks;
}
void Game::RemoveBlock(class Block* b)
{
	auto iter = std::find(mBlocks.begin(), mBlocks.end(), b);
	if (iter != mBlocks.end())
	{
		mBlocks.erase(iter);
	}
}
void Game::AddBlock(class Block* b)
{
	mBlocks.emplace_back(b);
}
void Game::ReadFile(std::string fileName)
{
	std::ifstream file(fileName);
	if (file.is_open())
	{
		std::string line;
		int lineCount = 1;
		while (std::getline(file, line))
		{
			for (int i = 0; i < line.length(); i++)
			{
				char symbol = line[i];
				Vector3 pos;
				pos.x = mPlayer->GetComponent<PlayerMove>()->GetBlockPos();
				pos.y = -237.5f + (25.0f * i);
				pos.z = 237.5f - (25.0f * lineCount);

				if (symbol == 'A')
				{
					Block* b = new Block(this, 3);
					b->SetPosition(pos);
				}
				if (symbol == 'B')
				{
					Block* b = new Block(this, 4);
					b->SetPosition(pos);
					b->SetExplode(true);
				}
			}
			lineCount++;
		}
		file.close();
	}
}