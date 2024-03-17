#include "Soldier.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "SoldierAI.h"
#include "EnemyComponent.h"
#include "Effect.h"

Soldier::Soldier(class Game* game, Vector2 position, class PathNode* start, class PathNode* end)
: Actor(game)
, mCollision(new CollisionComponent(this))
, mAnim(new AnimatedSprite(this))
, mAI(new SoldierAI(this))
{
	mGame = game;
	mCollision->SetSize(32.0f, 32.0f);
	mAnim->LoadAnimations("Assets/Soldier");
	mAnim->SetAnimation("WalkDown");
	mAnim->SetAnimFPS(5.0f);
	this->SetPosition(position);
	mAI->Setup(start, end);
	mEnemyComp = new EnemyComponent(this);
	mEnemyComp->SetHitPoints(2);

	GetComponent<EnemyComponent>()->SetOnDamage([this]() {
		mAI->SetIsStunned(true);
		mAI->SetStunTime(mAI->GetStunDuration());
		new Effect(GetGame(), GetPosition(), "Hit", "EnemyHit.wav");
	});

	GetComponent<EnemyComponent>()->SetOnDeath([this]() {
		new Effect(GetGame(), GetPosition(), "Death", "EnemyDie.wav");
	});
}