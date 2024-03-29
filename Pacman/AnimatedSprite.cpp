#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (!mAnimName.empty())
	{
		if (!mIsPaused)
		{
			mAnimTimer += mAnimFPS * deltaTime;
		}
		while (mAnimTimer >= mAnims[mAnimName].size())
		{
			mAnimTimer -= mAnims[mAnimName].size();
		}
		SetTexture((mAnims[mAnimName])[static_cast<int>(mAnimTimer)]);
	}
}

void AnimatedSprite::AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
