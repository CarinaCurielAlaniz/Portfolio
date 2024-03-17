#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "AudioSystem.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void EmRunIteration()
	{
		if (!mIsRunning)
		{
			emscripten_cancel_main_loop();
		}
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	class Player* GetPlayer() { return mPlayer; }
	AudioSystem* GetAudio() { return mAudio; }
	void AddBlock(class Block* b);
	std::vector<class Block*>& GetBlock();
	void RemoveBlock(class Block* b);
	class Renderer* GetRenderer() { return mRenderer; }
	SoundHandle GetSound() const { return mSH; }
	void ReadFile(std::string fileName);
	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;

	class Renderer* mRenderer = nullptr;
	AudioSystem* mAudio = nullptr;

	Uint32 mTicksCount = 0;
	class Player* mPlayer = nullptr;
	bool mIsRunning = true;
	std::vector<class Block*> mBlocks;
	SoundHandle mSH;
};
