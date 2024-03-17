#include "Collider.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"

Collider::Collider(class Game* game, float width, float height, Vector2 position)
: Actor(game)
, mCollision(new CollisionComponent(this))
{
	mGame = game;
	mCollision->SetSize(width, height);
	mGame->GetColliders().push_back(this);
	this->SetPosition(position);
}
void Collider::OnUpdate(float deltaTime)
{
}