#include "SoldierAI.h"
#include "Actor.h"
#include "Game.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <algorithm>
#include "EnemyComponent.h"
#include "Effect.h"

SoldierAI::SoldierAI(Actor* owner)
: SpriteComponent(owner)
{
	SetIsVisible(false); // Uncomment to hide debug paths
	mCurrDirection.x = 0.0f;
	mCurrDirection.y = 0.0f;
	mStunTime = 0.0f;
	mIsStunned = false;
	mPathF = owner->GetGame()->GetPathFinder();
}

void SoldierAI::Setup(PathNode* start, PathNode* end)
{
	mPatrolStart = start;
	mPatrolEnd = end;
	mPathF->CalculatePath(mPatrolStart, mPatrolEnd, mPath);
	mPrev = mPatrolStart;
	mNext = mPath.back();
	mPath.pop_back();
}
void SoldierAI::FindDirection(Vector2& vec)
{
	if (mNext != nullptr)
	{
		vec = mNext->GetPosition() - mPrev->GetPosition();
		vec.Normalize();
	}
	AnimatedSprite* animSprite = GetOwner()->GetComponent<AnimatedSprite>();
	if (vec.x == 1 && vec.y == 0)
	{

		animSprite->SetAnimation("WalkRight");
	}
	if (vec.x == -1 && vec.y == 0)
	{

		animSprite->SetAnimation("WalkLeft");
	}
	if (vec.y == 1 && vec.x == 0)
	{

		animSprite->SetAnimation("WalkDown");
	}
	if (vec.y == -1 && vec.x == 0)
	{
		animSprite->SetAnimation("WalkUp");
	}
}
void SoldierAI::Update(float deltaTime)
{
	if (mIsStunned)
	{
		mStunTime -= deltaTime;
	}
	if (mStunTime <= 0.0f)
	{
		mIsStunned = false;
		Vector2 pos = this->GetOwner()->GetPosition();
		FindDirection(mCurrDirection);
		pos += mCurrDirection * (SOLDIER_SPEED * deltaTime);
		if (Vector2::Distance(pos, mNext->GetPosition()) <= 1.0f)
		{
			pos = mNext->GetPosition();
			if (!mPath.empty())
			{
				mPrev = mNext;
				mNext = mPath.back();
				mPath.pop_back();
			}
			else
			{
				Setup(mPatrolEnd, mPatrolStart);
			}
		}
		this->GetOwner()->SetPosition(pos);
	}
}

// This helper is to just debug draw the soldier's path to visualize it
// (only called if this component is set to visible)
void SoldierAI::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

	// Draw from current position to next
	SDL_RenderDrawLine(renderer, static_cast<int>(mOwner->GetPosition().x - cameraPos.x),
					   static_cast<int>(mOwner->GetPosition().y - cameraPos.y),
					   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
					   static_cast<int>(mNext->GetPosition().y - cameraPos.y));

	// Draw from next to first node on path
	if (!mPath.empty())
	{
		// Draw from current position to next
		SDL_RenderDrawLine(renderer, static_cast<int>(mNext->GetPosition().x - cameraPos.x),
						   static_cast<int>(mNext->GetPosition().y - cameraPos.y),
						   static_cast<int>(mPath.back()->GetPosition().x - cameraPos.x),
						   static_cast<int>(mPath.back()->GetPosition().y - cameraPos.y));
	}

	// Draw each node on the path
	if (mPath.size() > 1)
	{
		for (size_t i = 0; i < mPath.size() - 1; i++)
		{
			SDL_RenderDrawLine(renderer, static_cast<int>(mPath[i]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i]->GetPosition().y - cameraPos.y),
							   static_cast<int>(mPath[i + 1]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i + 1]->GetPosition().y - cameraPos.y));
		}
	}
}
