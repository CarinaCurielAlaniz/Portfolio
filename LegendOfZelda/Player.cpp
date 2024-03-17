#include "Player.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "WrappingMove.h"
#include "PlayerMove.h"

Player::Player(class Game* game)
: Actor(game)
, mAnimSprite(new AnimatedSprite(this, 200))
, mCollision(new CollisionComponent(this))
, mMove(new PlayerMove(this))
{
	mGame = game;
	mCollision->SetSize(20, 20);
	mAnimSprite->LoadAnimations("Assets/Link");
	mAnimSprite->SetAnimation("StandDown");
}
void Player::OnProcessInput(const Uint8* keyState)
{
}
void Player::OnUpdate(float deltaTime)
{
}