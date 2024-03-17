#include "Sword.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "EnemyComponent.h"

Sword::Sword(class Game* game, Vector2 position)
: Actor(game)
, mCollision(new CollisionComponent(this))
{
	mGame = game;
	mCollision->SetSize(28.0f, 28.0f);
	this->SetPosition(position);
}