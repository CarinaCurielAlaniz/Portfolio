#pragma once
#include "Component.h"
#include "SDL2/SDL.h"
#include <string>
#include <vector>
#include "SpriteComponent.h"
class TiledBGComponent : public SpriteComponent
{
public:
	TiledBGComponent(class Actor* owner, int drawOrder = 50);
	~TiledBGComponent();
	void Draw(SDL_Renderer* renderer) override;
	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
	void SetTexture(SDL_Texture* texture) override;

protected:
	SDL_Texture* mTexture;
	std::vector<std::vector<int>> mCSVInts;
	int mTileWidth;
	int mTileHeight;
	int mDrawOrder;
};
