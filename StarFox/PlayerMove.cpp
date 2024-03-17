#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "SideBlock.h"
#include "Random.h"
#include "Bullet.h"
#include "Block.h"
#include "AudioSystem.h"
#include "HUD.h"
PlayerMove::PlayerMove(class Actor* owner)
: MoveComponent(owner)
{
	mVelocity.x = 400.0f;
	mSideBlock = 0.0f;
	mBlockPos = 0.0f;
	mFileNum = 1;
	mFileBlockPos = 1000.0f;
	mCoolDown = 1.0f;
	mPeggyCoolDown = 0.0f;
	mIsPlaying = false;
	mGame = mOwner->GetGame();
}
void PlayerMove::Update(float deltaTime)
{
	mTenSecondTimer -= deltaTime;
	if (mTenSecondTimer <= 0.0f)
	{
		mMultSpeed += 0.15f;
		mTenSecondTimer = 10.0f;
	}
	mPos = mOwner->GetPosition();
	mPos += (mVelocity * mMultSpeed) * deltaTime;
	mPos.y = Math::Clamp(mPos.y, -180.0f, 180.0f);
	mPos.z = Math::Clamp(mPos.z, -225.0f, 225.0f);
	mOwner->SetPosition(mPos);

	Vector3 mCameraPos = mPos - mOwner->GetForward() * 300.0f;
	Vector3 mTargetPos = mPos + mOwner->GetForward() * 20.0f;
	mCameraPos.z = 0.0f;
	Matrix4 mActorCamera = Matrix4::CreateLookAt(mCameraPos, mTargetPos, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(mActorCamera);

	mFoward = 4000.0f * mOwner->GetForward().x + mPos.x;
	DrawSideBlocks();
	LoadBlocks();
	BlockCollide(deltaTime);
	mPeggyCoolDown -= deltaTime;
	Shield();
	BarrelRoll(deltaTime);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	mVelocity.z = 0.0f;
	mVelocity.y = 0.0f;
	if (keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S])
	{
		mVelocity.z += 300.0f;
	}
	if (keyState[SDL_SCANCODE_S] && !keyState[SDL_SCANCODE_W])
	{
		mVelocity.z -= 300.0f;
	}

	if (keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D])
	{
		mVelocity.y -= 300.0f;
	}
	if (keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A])
	{
		mVelocity.y += 300.0f;
	}

	if (keyState[SDL_SCANCODE_SPACE] && !mLastFrame)
	{
		new Bullet(mOwner->GetGame());
		mLastFrame = false;
		mOwner->GetGame()->GetAudio()->PlaySound("Shoot.wav");
	}
	if (keyState[SDL_SCANCODE_Q] && !mIsRolling)
	{
		Player* ship = mOwner->GetGame()->GetPlayer();
		int shield = ship->GetShield();
		if (shield < 3)
		{
			shield++;
			ship->SetShield(shield);
		}
		mIsRolling = true;
		mOwner->GetGame()->GetAudio()->PlaySound("Boost.wav");
	}
	mLastFrame = keyState[SDL_SCANCODE_SPACE];
}
void PlayerMove::DrawSideBlocks()
{
	while (mBlockPos <= mFoward)
	{
		SideBlock* s1 = new SideBlock(mGame, mLeftRightIdx);
		Vector3 pos = Vector3::Zero;
		pos.y = 500.0f;
		pos.x = mSideBlock;
		s1->SetPosition(pos);
		s1->SetRotation(Math::Pi);
		SideBlock* s2 = new SideBlock(mGame, mLeftRightIdx);
		pos.y = -500.0f;
		s2->SetPosition(pos);
		SideBlock* s3 = new SideBlock(mGame, 5);
		Vector3 pos2 = Vector3::Zero;
		pos2.z = -500.0f;
		pos2.x = mSideBlock;
		s3->SetPosition(pos2);
		SideBlock* s4 = new SideBlock(mGame, mTopIdx);
		pos2.z = 500.0f;
		s4->SetPosition(pos2);
		mSideBlock += 500.0f;
		mBlockPos += 500.0f;

		if (mLeftRightIdx == 2)
		{
			mLeftRightIdx = 0;
		}
		else
		{
			mLeftRightIdx++;
		}
		if (mTopIdx == 7)
		{
			mTopIdx = 6;
		}
		else
		{
			mTopIdx++;
		}
	}
}
void PlayerMove::LoadBlocks()
{
	while (mFileBlockPos <= mFoward)
	{
		std::string fileName = "Assets/Blocks/";
		fileName += std::to_string(mFileNum);
		fileName += ".txt";
		mOwner->GetGame()->ReadFile(fileName);

		if (mPastTwenty)
		{
			mFileNum = Random::GetIntRange(1, 20);
		}
		else
		{
			if (mFileNum < 20)
			{
				mFileNum++;
			}
			else
			{
				mPastTwenty = true;
				mFileNum = Random::GetIntRange(1, 20);
			}
		}
		mFileBlockPos += 1000.0f;
	}
}
void PlayerMove::BlockCollide(float deltaTime)
{
	std::vector<Block*> blockCop = mGame->GetBlock();
	bool playSound = false;
	for (int i = 0; i < blockCop.size(); i++)
	{
		bool doesCollide =
			mOwner->GetComponent<CollisionComponent>()->Intersect(blockCop[i]->GetCol());
		if (doesCollide)
		{
			blockCop[i]->SetState(ActorState::Destroy);
			mPlayerCollides = true;
			mCollide = true;
			playSound = true;
		}
	}
	if (mCollide)
	{
		mCoolDown -= deltaTime;
	}
	if (playSound)
	{
		mOwner->GetGame()->GetAudio()->PlaySound("ShipHit.wav");
	}
}
void PlayerMove::Shield()
{
	if (mGame->GetPlayer()->GetShield() < 3 && mPeggyCoolDown <= 0.0f)
	{
		mOwner->GetGame()->GetPlayer()->GetHud()->DoABarrelRoll();
		mPeggyCoolDown = Random::GetFloatRange(15.0f, 25.0f);
	}
	if ((mPlayerCollides && mCoolDown <= 0.0f) ||
		(mPlayerCollides && mGame->GetPlayer()->GetShield() - 1 == 0))
	{
		mCollide = false;
		mCoolDown = 1.0f;
		int shield = mGame->GetPlayer()->GetShield();
		shield--;
		if (shield == 1)
		{
			mDamageSH = mOwner->GetGame()->GetAudio()->PlaySound("DamageAlert.ogg", true);
			mIsPlaying = true;
		}
		if (shield <= 0)
		{
			mOwner->GetGame()->GetAudio()->PauseSound(mOwner->GetGame()->GetSound());
			mOwner->GetGame()->GetAudio()->PauseSound(mDamageSH);
			mOwner->GetGame()->GetAudio()->PlaySound("ShipDie.wav");
			mGame->GetPlayer()->SetState(ActorState::Paused);
		}
		mGame->GetPlayer()->SetShield(shield);
		mPlayerCollides = false;
	}
	if (mGame->GetPlayer()->GetShield() > 1 && mIsPlaying)
	{
		mOwner->GetGame()->GetAudio()->PauseSound(mDamageSH);
		mIsPlaying = false;
	}
}
void PlayerMove::BarrelRoll(float deltaTime)
{
	if (mIsRolling)
	{
		mRollLifetime -= deltaTime;
		mRoll *= (4.0f * Math::Pi);
		mGame->GetPlayer()->SetRollRotation(mRoll);
		if (mRollLifetime <= 0.0f)
		{
			mRollLifetime = 0.5f;
			mIsRolling = false;
			mGame->GetPlayer()->SetRollRotation(0.0f);
			mRoll = 90.0f;
		}
	}
}