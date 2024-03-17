//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "SDL2/SDL.h"
#include <map>
#include "Math.h"
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include "AudioSystem.h"

class Game
{
public:
	Game();
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void ReadFile(std::string fileName);
	SDL_Texture* GetTexture(const std::string& key);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	Vector2& GetCameraPos();
	std::vector<class Collider*>& GetColliders();
	int GetBackgroundChannel() const;
	class CollisionComponent* GetColComp();
	void LoadCSV(std::string fileName);
	class AudioSystem* GetAudio() { return mAudio; }

	std::vector<class EnemyComponent*>& GetEnemies();

	void AddEnemies(class EnemyComponent* e);
	void RemoveEnemies(class EnemyComponent* e);
	class PathFinder* GetPathFinder();
	//constants
	static const int WINDOW_WIDTH = 512;
	static const int WINDOW_HEIGHT = 448;

private:
	//functions
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	//data
	std::vector<class Actor*> mActors;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mPrevTick;
	bool mGameIsRunning;
	int mDirection;
	std::unordered_map<std::string, SDL_Texture*> mTextureMap;
	std::vector<class SpriteComponent*> mSprites;
	float mDeltaTime;
	int mImgInt;
	Vector2 mCameraPos;
	class Player* mPlayer;
	class CollisionComponent* mCol;
	std::vector<class Collider*> mCollides;
	class PathFinder* mPathFinder;
	std::vector<class EnemyComponent*> mEnemies;
	class AudioSystem* mAudio;
	bool mStart;
	bool mIsLooping = false;
	class SoundHandle mSoundHandle;
};