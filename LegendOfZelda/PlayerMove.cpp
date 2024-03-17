#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Collider.h"
#include "AnimatedSprite.h"
#include "SpriteComponent.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "Sword.h"
#include "AudioSystem.h"

PlayerMove::PlayerMove(class Actor* owner)
: MoveComponent(owner)
, mSword(new Sword(owner->GetGame(), owner->GetPosition()))
{
	mActor = owner;
	mGame = mOwner->GetGame();
	mPlayer = static_cast<Player*>(mOwner);
	mLastFrame = false;
	mIsSwinging = false;
	mAttackLife = 0.25f;
	mSpeed.x = 0.0f;
	mSpeed.y = 0.0f;
	mIsWalking = false;
	mAnim = mActor->GetComponent<AnimatedSprite>();
}
void PlayerMove::Update(float deltaTime)
{
	mPos = mActor->GetPosition();
	mPos += mSpeed * deltaTime;
	if (mIsSwinging)
	{
		mAttackLife -= deltaTime;
		SwordAttack();
		if (mAttackLife <= 0.0f)
		{
			mIsSwinging = false;
			IdleAnims();
			mAttackLife = 0.25f;
		}
	}
	Vector2 offset1;
	Vector2 dummyOffset;
	std::vector<class EnemyComponent*>& enemiesVec = mGame->GetEnemies();
	for (size_t i = 0; i < enemiesVec.size(); i++)
	{
		if (mIsSwinging)
		{
			CollSide swordCollide = mSword->GetCol()->GetMinOverlap(enemiesVec[i]->GetCol(),
																	dummyOffset);
			if (swordCollide != CollSide::None)
			{
				enemiesVec[i]->TakeDamage();
			}
		}
		CollSide collide = mPlayer->GetCol()->GetMinOverlap(enemiesVec[i]->GetCol(), offset1);
		if (collide != CollSide::None)
		{
			mPos += offset1;
		}
	}
	mActor->SetPosition(mPos);
	Vector2 offset;
	std::vector<class Collider*>& colliderVec = mGame->GetColliders();
	for (size_t i = 0; i < colliderVec.size(); i++)
	{
		CollSide collide = mPlayer->GetCol()->GetMinOverlap(colliderVec[i]->GetCol(), offset);
		if (collide != CollSide::None)
		{
			mPos += offset;
		}
	}
	mActor->SetPosition(mPos);
	Vector2 tempPos = mActor->GetPosition();
	Vector2& cameraPos = mOwner->GetGame()->GetCameraPos();
	tempPos.x += -256.0f;
	tempPos.y += -224.0f;
	cameraPos = tempPos;
}
void PlayerMove::ProcessInput(const Uint8* keyState)
{
	mSpeed.x = 0.0f;
	mSpeed.y = 0.0f;
	mIsWalking = false;

	if (keyState[SDL_SCANCODE_D] && !mIsSwinging)
	{
		mDirection.x = 1;
		mSpeed.x = 150.0f;
		mIsWalking = true;
		mLastDirection = "D";
	}
	else if (keyState[SDL_SCANCODE_A] && !mIsSwinging)
	{
		mDirection.x = -1;
		mSpeed.x = -150.0f;
		mIsWalking = true;
		mLastDirection = "A";
	}
	else if (keyState[SDL_SCANCODE_W] && !mIsSwinging)
	{
		mIsWalking = true;
		mDirection.y = -1;
		mSpeed.y = -150.0f;
		mLastDirection = "W";
	}
	else if (keyState[SDL_SCANCODE_S] && !mIsSwinging)
	{
		mDirection.y = 1;
		mSpeed.y = 150.0f;
		mIsWalking = true;
		mLastDirection = "S";
	}
	else if (!mIsSwinging)
	{
		IdleAnims();
	}
	if (keyState[SDL_SCANCODE_SPACE] && !mLastFrame && mAttackLife >= 0.25f)
	{
		SwordAnim();
		mIsSwinging = true;
	}
	if (mIsWalking && !mIsSwinging)
	{
		WalkingAnims();
	}
	mLastFrame = keyState[SDL_SCANCODE_SPACE];
}
void PlayerMove::SwordAnim()
{
	mAnim->ResetAnimTimer();
	mGame->GetAudio()->PlaySound("SwordSlash.wav");
	if (mLastDirection == "D")
	{
		mAnim->SetAnimation("AttackRight");
	}
	else if (mLastDirection == "A")
	{
		mAnim->SetAnimation("AttackLeft");
	}
	else if (mLastDirection == "W")
	{
		mAnim->SetAnimation("AttackUp");
	}
	else
	{
		mAnim->SetAnimation("AttackDown");
	}
}
void PlayerMove::WalkingAnims()
{
	if (mLastDirection == "D")
	{
		mAnim->SetAnimation("WalkRight");
	}
	else if (mLastDirection == "A")
	{
		mAnim->SetAnimation("WalkLeft");
	}
	else if (mLastDirection == "W")
	{
		mAnim->SetAnimation("WalkUp");
	}
	else
	{
		mAnim->SetAnimation("WalkDown");
	}
}
void PlayerMove::IdleAnims()
{

	if (mLastDirection == "D")
	{
		mAnim->SetAnimation("StandRight");
	}
	else if (mLastDirection == "A")
	{
		mAnim->SetAnimation("StandLeft");
	}
	else if (mLastDirection == "W")
	{
		mAnim->SetAnimation("StandUp");
	}
	else
	{
		mAnim->SetAnimation("StandDown");
	}
}
void PlayerMove::SwordAttack()
{
	Vector2 offset;
	offset.x = 0.0f;
	offset.y = 0.0f;
	if (mLastDirection == "D")
	{
		offset.x = 32.0f;
		mSword->SetPosition(mPos + offset);
		mSword->GetCol()->SetSize(28.0f, 20.0f);
	}
	else if (mLastDirection == "A")
	{
		offset.x = -32.0f;
		mSword->SetPosition(mPos + offset);
		mSword->GetCol()->SetSize(28.0f, 20.0f);
	}
	else if (mLastDirection == "W")
	{
		offset.y = -40.0f;
		mSword->SetPosition(mPos + offset);
		mSword->GetCol()->SetSize(20.0f, 28.0f);
	}
	else
	{
		offset.y = 40.0f;
		mSword->SetPosition(mPos + offset);
		mSword->GetCol()->SetSize(20.0f, 28.0f);
	}
}