#include "TiledBGComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>
#include <string>

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner)
, mDrawOrder(drawOrder)
{
	mTexture = nullptr;
	mTileWidth = 0;
	mTileHeight = 0;
	mOwner->GetGame()->AddSprite(this);
}

TiledBGComponent::~TiledBGComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void TiledBGComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

void TiledBGComponent::Draw(SDL_Renderer* renderer)
{
	for (int i = 0; i < mCSVInts.size(); i++)
	{
		for (int j = 0; j < mCSVInts[i].size(); j++)
		{
			if (mCSVInts[i][j] != -1)
			{
				//SDL_Log("Num- %d", mCSVInts[i][j]);
				int tilesPerRow = GetTexWidth() / mTileWidth;

				SDL_Rect r;
				r.w = mTileWidth;
				r.h = mTileHeight;

				r.x = static_cast<int>((j * mTileWidth) - mOwner->GetGame()->GetCameraPos().x);
				r.y = static_cast<int>((i * mTileHeight) - mOwner->GetGame()->GetCameraPos().y);

				SDL_Rect srcRect;

				srcRect.w = mTileWidth;
				srcRect.h = mTileHeight;

				srcRect.x = ((mCSVInts[i][j]) % tilesPerRow) * mTileHeight;
				srcRect.y = ((mCSVInts[i][j]) / tilesPerRow) * mTileWidth;

				SDL_RenderCopyEx(renderer, mTexture, &srcRect, &r, 0.0, nullptr, SDL_FLIP_NONE);
			}
		}
	}
}
void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
	mTileWidth = tileWidth;
	mTileHeight = tileHeight;

	std::ifstream file(fileName);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			if (!line.empty())
			{
				std::vector<std::string> stringInts = CSVHelper::Split(line);
				std::vector<int> lineOfInts;
				for (int i = 0; i < stringInts.size(); i++)
				{
					lineOfInts.push_back(std::stoi(stringInts[i]));
				}
				mCSVInts.push_back(lineOfInts);
			}
		}
		file.close();
	}
}
