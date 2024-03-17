#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"

GhostAI::GhostAI(class Actor* owner)
: Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
	mCollision = owner->GetComponent<CollisionComponent>();
	mCurrDirection.x = 0.0f;
	mCurrDirection.y = 0.0f;
	mTime = 0.0f;
	mAnimIsSet = false;
}

void GhostAI::Update(float deltaTime)
{
	float mForwardSpeed = 0.0f;
	mTime += deltaTime;
	if (mState == Scatter || mState == Chase)
	{
		mForwardSpeed = 90.0f;
	}
	if (mState == Frightened)
	{
		mForwardSpeed = 65.0f;
	}
	if (mState == Dead)
	{
		mForwardSpeed = 125.0f;
	}
	Vector2 pos = mOwner->GetPosition();
	pos += (mCurrDirection) * (mForwardSpeed * deltaTime);
	mOwner->SetPosition(pos);
	if (mGhost->GetType() == Ghost::Blinky)
	{
		mBlinkyPos = mGhost->GetPosition();
	}
	bool collide = mCollision->Intersect(mNextNode->GetComponent<CollisionComponent>());
	if (collide)
	{
		mOwner->SetPosition(mNextNode->GetPosition());
		ChangeState();
		UpdateTargetNode();
		UpdatePrevAndNext();
		FindDirection(mCurrDirection);
	}
}
void GhostAI::ChangeState()
{
	if (mState == Frightened && mTime >= 7.0f)
	{
		mState = Scatter;
		mTime = 0.0f;
	}
	if (mState == Dead && mNextNode == mGhost->GetGame()->GetGhostPen())
	{
		mState = Scatter;
		mTime = 0.0f;
	}
	if (mState == Scatter && mTime >= 5.0f)
	{
		mState = Chase;
		mTime = 0.0f;
	}
	if (mState == Chase && mTime >= 20.0f)
	{
		mState = Scatter;
		mTime = 0.0f;
	}
}
void GhostAI::UpdateTargetNode()
{
	if (mState == Scatter)
	{
		mTargetNode = mGhost->GetScatterNode();
	}
	if (mState == Frightened)
	{
		int size = static_cast<int>(mNextNode->mAdjacent.size());
		std::vector<PathNode*> validOptions;

		for (int i = 0; i < size; i++)
		{
			PathNode* potTarget = mNextNode->mAdjacent[i];
			if (potTarget != mPrevNode && potTarget->GetType() != PathNode::Ghost &&
				potTarget->GetType() != PathNode::Tunnel)
			{
				validOptions.push_back(potTarget);
			}
		}

		int numValidOptions = static_cast<int>(validOptions.size());
		if (numValidOptions > 0)
		{
			size_t randIdx = Random::GetIntRange(0, numValidOptions - 1);
			mTargetNode = validOptions[randIdx];
		}
	}
	if (mState == Dead)
	{

		mTargetNode = mGhost->GetGame()->GetGhostPen();
	}
	if (mState == Chase)
	{
		if (mGhost->GetType() == Ghost::Blinky)
		{
			FindClosestDefaultNode();
			mBlinkyPos = mGhost->GetPosition();
		}
		if (mGhost->GetType() == Ghost::Pinky)
		{
			PacMan* pac = mGhost->GetGame()->GetPlayer();
			PathNode* potTarget = pac->GetPrevNode();
			Vector2 point = pac->GetPointInFrontOf(80);
			FindNearestNode(point, potTarget);
			mTargetNode = potTarget;
		}
		if (mGhost->GetType() == Ghost::Inky)
		{
			PacMan* pac = mGhost->GetGame()->GetPlayer();
			Vector2 point = pac->GetPointInFrontOf(40);
			PathNode* potTarget = pac->GetPrevNode();
			Vector2 v = point - mBlinkyPos;
			v *= 2;
			FindNearestNode(v, potTarget);
			mTargetNode = potTarget;
		}
		if (mGhost->GetType() == Ghost::Clyde)
		{
			PacMan* pac = mGhost->GetGame()->GetPlayer();
			Vector2 curPos = pac->GetPosition();
			float dist = Vector2::Distance(curPos, mGhost->GetPosition());
			if (dist > 150.0f)
			{
				FindClosestDefaultNode();
			}
			else
			{
				mTargetNode = mGhost->GetScatterNode();
			}
		}
	}
}

void GhostAI::FindNearestNode(Vector2 compPoint, PathNode*& potentialTarget)
{
	float minDist = std::numeric_limits<float>::max();
	std::vector<PathNode*> allNodes = mGhost->GetGame()->GetPathNodes();
	for (int i = 0; i < allNodes.size(); i++)
	{
		float maybeDist = Vector2::Distance(compPoint, allNodes[i]->GetPosition());
		if (maybeDist < minDist && allNodes[i]->GetType() == PathNode::Default)
		{
			minDist = maybeDist;
			potentialTarget = allNodes[i];
		}
	}
}
void GhostAI::FindClosestDefaultNode()
{
	PacMan* pac = mGhost->GetGame()->GetPlayer();
	PathNode* potTarget = pac->GetPrevNode();
	if (potTarget->GetType() == PathNode::Tunnel)
	{
		Vector2 curPos = pac->GetPosition();
		FindNearestNode(curPos, potTarget);
	}
	mTargetNode = potTarget;
}

void GhostAI::UpdatePrevAndNext()
{
	PathNode* newNext = nullptr;
	float minDist = std::numeric_limits<float>::max();
	//3 for loops
	//attempt 1
	bool found = false;
	if (!found)
	{
		for (int i = 0; i < mNextNode->mAdjacent.size(); i++)
		{
			PathNode* maybeNext = mNextNode->mAdjacent[i];
			if (maybeNext != mPrevNode &&
				(maybeNext->GetType() != PathNode::Ghost || (mState == Dead)) &&
				maybeNext->GetType() != PathNode::Tunnel)
			{
				float maybeDist = Vector2::Distance(mTargetNode->GetPosition(),
													maybeNext->GetPosition());
				if (maybeDist < minDist)
				{
					newNext = maybeNext;
					minDist = maybeDist;
					found = true;
				}
			}
		}
	}
	if (!found)
	{
		for (int i = 0; i < mNextNode->mAdjacent.size(); i++)
		{
			PathNode* maybeNext = mNextNode->mAdjacent[i];
			if (maybeNext != mPrevNode && maybeNext->GetType() != PathNode::Tunnel)
			{
				float maybeDist = Vector2::Distance(mTargetNode->GetPosition(),
													maybeNext->GetPosition());
				if (maybeDist < minDist)
				{
					newNext = maybeNext;
					minDist = maybeDist;
					found = true;
				}
			}
		}
	}
	if (!found)
	{
		for (int i = 0; i < mNextNode->mAdjacent.size(); i++)
		{
			PathNode* maybeNext = mNextNode->mAdjacent[i];
			if (maybeNext->GetType() != PathNode::Tunnel)
			{
				float maybeDist = Vector2::Distance(mTargetNode->GetPosition(),
													maybeNext->GetPosition());
				if (maybeDist < minDist)
				{
					newNext = maybeNext;
					minDist = maybeDist;
				}
			}
		}
	}

	mPrevNode = mNextNode;
	mNextNode = newNext;
}

void GhostAI::Frighten()
{
	if (mState != Dead)
	{
		mTime = 0.0f;
		if (mState != Frightened)
		{
			mState = Frightened;
			PathNode* temp = mPrevNode;
			mPrevNode = mNextNode;
			mNextNode = temp;
			FindDirection(mCurrDirection);
			mTargetNode = nullptr;
		}
	}
}

void GhostAI::Start(PathNode* startNode)
{
	mTime = 0.0f;
	mOwner->SetPosition(startNode->GetPosition());
	mState = Scatter;
	mPrevNode = nullptr;
	mNextNode = startNode;
}

void GhostAI::Die()
{
	mTime = 0.0f;
	mState = Dead;
	FindDirection(mCurrDirection);
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render, static_cast<int>(mOwner->GetPosition().x),
						   static_cast<int>(mOwner->GetPosition().y),
						   static_cast<int>(mNextNode->GetPosition().x),
						   static_cast<int>(mNextNode->GetPosition().y));
	}
}
void GhostAI::FindDirection(Vector2& vec)
{
	if (mNextNode != nullptr)
	{
		vec = mNextNode->GetPosition() - mPrevNode->GetPosition();
		vec.Normalize();
	}
	AnimatedSprite* animSprite = mGhost->GetComponent<AnimatedSprite>();
	if (vec.x == 1 && vec.y == 0)
	{
		if (mState != Dead)
		{
			animSprite->SetAnimation("right");
		}
		else
		{

			animSprite->SetAnimation("deadright");
		}
	}
	if (vec.x == -1 && vec.y == 0)
	{
		if (mState != Dead)
		{
			animSprite->SetAnimation("left");
		}
		else
		{

			animSprite->SetAnimation("deadleft");
		}
	}
	if (vec.y == 1 && vec.x == 0)
	{
		if (mState != Dead)
		{
			animSprite->SetAnimation("down");
		}
		else
		{

			animSprite->SetAnimation("deaddown");
		}
	}
	if (vec.y == -1 && vec.x == 0)
	{
		if (mState != Dead)
		{
			animSprite->SetAnimation("up");
		}
		else
		{

			animSprite->SetAnimation("deadup");
		}
	}
	if (mState == Frightened)
	{
		if (mTime < 5.0f)
		{
			if (mGhost->GetComponent<AnimatedSprite>()->GetAnimName() != "scared0")
			{
				mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
			}
		}
		else
		{
			if (mGhost->GetComponent<AnimatedSprite>()->GetAnimName() != "scared1")
			{
				mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
			}
		}
	}
}