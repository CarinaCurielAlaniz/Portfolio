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

private:
	Vector2 mOldPos;
	Vector2 mOldCamera;
	Actor* mActor;
	Vector2 mSpeed;
	std::string mLastDirection;
	Vector2 mDirection;
	Game* mGame;
	class Player* mPlayer;
	Vector2 mPos;
	bool mLastFrame;
	float mAttackLife;
	bool mIsSwinging;
	class AnimatedSprite* mAnim;
	void IdleAnims();
	void SwordAnim();
	class Sword* mSword;
	void SwordAttack();
	bool mIsWalking;
	void WalkingAnims();
};