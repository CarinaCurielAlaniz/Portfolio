//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "PathFinder.h"
#include "PlayerMove.h"
#include "Player.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "WrappingMove.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include "TiledBGComponent.h"
#include "CSVHelper.h"
#include "Soldier.h"
#include "Bush.h"
#include "Collider.h"
#include "PathNode.h"
#include "AudioSystem.h"

Game::Game()
{
	mGameIsRunning = true;
	mPrevTick = 0;
	mDirection = 0;
	mWindow = nullptr;
	mRenderer = nullptr;
	mDeltaTime = 0.0f;
	mImgInt = 0;
	mPlayer = nullptr;
	mCol = nullptr;
	mStart = true;
	mPathFinder = nullptr;
	mAudio = nullptr;
}

bool Game::Initialize()
{
	Random::Init();
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
	{
		return false;
	}
	mWindow = SDL_CreateWindow("Legend of Zelda: A Link to the Past", SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (mWindow == nullptr)
	{
		Shutdown();
		SDL_Quit();
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr)
	{
		Shutdown();
		SDL_Quit();
		return false;
	}
	mImgInt = IMG_Init(IMG_INIT_PNG);
	LoadData();
	mAudio = new AudioSystem();
	return true;
}

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	IMG_Quit();
	delete mAudio;
}

void Game::AddActor(class Actor* actor)
{
	mActors.push_back(actor);
}

void Game::RemoveActor(class Actor* actor)
{
	std::vector<class Actor*>::iterator it;
	it = find(mActors.begin(), mActors.end(), actor);
	if (it != mActors.end())
	{
		mActors.erase(it);
	}
}

void Game::RunLoop()
{
	while (mGameIsRunning)
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
			mGameIsRunning = false;
			break;
		default:
			break;
		}
	}
	const Uint8* keyArray = SDL_GetKeyboardState(NULL);
	if (keyArray[SDL_SCANCODE_ESCAPE])
	{
		mGameIsRunning = false;
	}

	std::vector<class Actor*> copyActors = mActors;
	for (size_t i = 0; i < copyActors.size(); i++)
	{
		copyActors[i]->ProcessInput(keyArray);
	}
}

void Game::UpdateGame()
{
	while (SDL_GetTicks() < (mPrevTick + 16))
	{
	}
	float dTime = (SDL_GetTicks() - mPrevTick) / 1000.0f;
	mPrevTick = SDL_GetTicks();
	if (dTime > 0.033)
	{
		dTime = static_cast<float>(0.033);
	}
	mDeltaTime = dTime;
	mAudio->Update(dTime);

	if (mStart)
	{
		mSoundHandle = mAudio->PlaySound("MusicStart.ogg");
		mStart = false;
	}
	if (mAudio->GetSoundState(mSoundHandle) == SoundState::Stopped && !mIsLooping)
	{
		SDL_Log("hi");
		mAudio->PlaySound("MusicLoop.ogg", true);
		mIsLooping = true;
	}

	std::vector<class Actor*> copyActors = mActors;
	for (size_t i = 0; i < copyActors.size(); i++)
	{
		copyActors[i]->Update(dTime);
	}
	std::vector<class Actor*> destroyActors;
	for (size_t i = 0; i < mActors.size(); i++)
	{
		if (mActors[i]->GetState() == ActorState::Destroy)
		{
			destroyActors.push_back(mActors[i]);
		}
	}
	for (size_t i = 0; i < destroyActors.size(); i++)
	{
		delete destroyActors[i];
	}
}

void Game::GenerateOutput()
{
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	for (size_t i = 0; i < mSprites.size(); i++)
	{
		if (mSprites[i]->IsVisible())
		{
			mSprites[i]->Draw(mRenderer);
		}
	}
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	Actor* background = new Actor(this);
	Vector2 pos3(3392, WINDOW_HEIGHT / 2);
	background->SetPosition(pos3);
	TiledBGComponent* tb = new TiledBGComponent(background);
	mPathFinder = new PathFinder(this);
	tb->SetTexture(GetTexture("Assets/Map/Tiles.png"));
	tb->LoadTileCSV("Assets/Map/Tiles.csv", 32, 32);
	LoadCSV("Assets/Map/Objects.csv");
}
void Game::UnloadData()
{
	while (!mActors.empty())
	{
		size_t back = mActors.size() - 1;
		delete mActors[back];
	}
	for (auto it = mTextureMap.begin(); it != mTextureMap.end();)
	{
		SDL_DestroyTexture(it->second);
		it = mTextureMap.erase(it);
	}
}

SDL_Texture* Game::GetTexture(const std::string& key)
{
	auto it = mTextureMap.find(key);
	if (it != mTextureMap.end())
	{
		return it->second;
	}
	else
	{
		const char* cKey = key.c_str();
		SDL_Surface* x = IMG_Load(cKey);
		if (x == nullptr)
		{
			SDL_Log("FAILED");
		}
		SDL_Texture* y = SDL_CreateTextureFromSurface(mRenderer, x);
		SDL_FreeSurface(x);
		mTextureMap.insert(std::make_pair(key, y));
		return y;
	}
}
void Game::AddSprite(class SpriteComponent* sprite)
{
	mSprites.push_back(sprite);
	std::sort(mSprites.begin(), mSprites.end(),
			  [](class SpriteComponent* a, class SpriteComponent* b) {
				  return a->GetDrawOrder() < b->GetDrawOrder();
			  });
}

void Game::RemoveSprite(class SpriteComponent* sprite)
{
	std::vector<class SpriteComponent*>::iterator it;
	it = find(mSprites.begin(), mSprites.end(), sprite);
	if (it != mSprites.end())
	{
		mSprites.erase(it);
	}
}
void Game::ReadFile(std::string fileName)
{
}
Vector2& Game::GetCameraPos()
{
	return mCameraPos;
}
class CollisionComponent* Game::GetColComp()
{
	return mCol;
}
void Game::LoadCSV(std::string fileName)
{
	std::ifstream file(fileName);
	if (file.is_open())
	{
		int counter = 0;
		std::string line;
		while (std::getline(file, line))
		{
			if (!line.empty())
			{
				float x = 0.0f;
				float y = 0.0f;
				float w = 0.0f;
				float h = 0.0f;
				Vector2 pos;
				std::vector<std::string> sepLine = CSVHelper::Split(line);
				if (counter != 0)
				{
					x = std::stof(sepLine[1]);
					y = std::stof(sepLine[2]);
					w = std::stof(sepLine[3]);
					h = std::stof(sepLine[4]);
					pos.x = static_cast<float>(x + (0.5 * w));
					pos.y = static_cast<float>(y + (0.5 * h));
				}
				if (sepLine[0] == "Player")
				{
					mPlayer = new Player(this);
					mPlayer->SetPosition(pos);
				}
				if (sepLine[0] == "Collider")
				{
					new Collider(this, w, h, pos);
				}
				if (sepLine[0] == "Bush")
				{
					new Bush(this, pos);
				}
				if (sepLine[0] == "Soldier")
				{

					PathNode* start = mPathFinder->GetPathNode(std::stoi(sepLine[5]),
															   std::stoi(sepLine[6]));
					PathNode* end = mPathFinder->GetPathNode(std::stoi(sepLine[7]),
															 std::stoi(sepLine[8]));
					new Soldier(this, pos, start, end);
				}
			}
			counter++;
		}
		file.close();
	}
}

std::vector<class Collider*>& Game::GetColliders()
{
	return mCollides;
}

PathFinder* Game::GetPathFinder()
{
	return mPathFinder;
}
std::vector<class EnemyComponent*>& Game::GetEnemies()
{
	return mEnemies;
}
void Game::AddEnemies(class EnemyComponent* e)
{
	mEnemies.push_back(e);
}
void Game::RemoveEnemies(class EnemyComponent* e)
{
	std::vector<class EnemyComponent*>::iterator it;
	it = find(mEnemies.begin(), mEnemies.end(), e);
	if (it != mEnemies.end())
	{
		mEnemies.erase(it);
	}
}