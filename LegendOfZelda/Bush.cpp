#include "Bush.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "EnemyComponent.h"
#include "PathFinder.h"
#include "Effect.h"

Bush::Bush(class Game* game, Vector2 position)
: Actor(game)
, mCollision(new CollisionComponent(this))
, mSprite(new SpriteComponent(this))
{
	mGame = game;
	mCollision->SetSize(32.0f, 32.0f);
	mSprite->SetTexture(game->GetTexture("Assets/Bush.png"));
	this->SetPosition(position);
	mEnemyComp = new EnemyComponent(this);
	mEnemyComp->SetHitPoints(1);
	mPathF = mGame->GetPathFinder();
}

void Bush::OnUpdate(float deltaTime)
{
	mEnemyComp->SetOnDeath([this]() {
		Vector2 pos = GetPosition();
		int row = static_cast<int>(pos.y / 32.0f);
		int col = static_cast<int>(pos.x / 32.0f);
		mPathF->SetIsBlocked(row, col, false);
		new Effect(GetGame(), pos, "BushDeath", "BushDie.wav");
	});
}
