#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"
#include <unordered_map>
#include "Game.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	float GetMultSpeed() const { return mMultSpeed; }
	void DrawSideBlocks();
	float GetBlockPos() const { return mFileBlockPos; }
	void LoadBlocks();
	void BlockCollide(float deltaTime);
	void Shield();
	void BarrelRoll(float deltaTime);

private:
	float mTenSecondTimer = 10.0f;
	float mPeggyCoolDown;
	Vector3 mVelocity;
	float mMultSpeed = 1.0f;
	Vector3 mPos;
	float mSideBlock;
	float mBlockPos;
	int mLeftRightIdx = 0;
	int mTopIdx = 6;
	int mFileNum;
	float mFileBlockPos;
	bool mPastTwenty = false;
	bool mLastFrame = false;
	bool mPlayerCollides = false;
	float mCoolDown;
	bool mCollide = false;
	bool mIsRolling = false;
	float mRoll = 90.0f;
	float mRollLifetime = 0.5;
	bool mIsPlaying;
	Game* mGame;
	float mFoward;

	SoundHandle mDamageSH;
};