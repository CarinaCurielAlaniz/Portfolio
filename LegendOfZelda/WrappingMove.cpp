#include "WrappingMove.h"
#include "Actor.h"

WrappingMove::WrappingMove(class Actor* owner)
: MoveComponent(owner)
{
	Vector2 dir;
	dir.x = 1;
	dir.y = 0;
	mDirection = dir;
	mForwardSpeed = 30.0f;
}
void WrappingMove::Update(float deltaTime)
{
	Vector2 pos = mOwner->GetPosition();
	pos.x += (mDirection.x) * mForwardSpeed * deltaTime;
	if (pos.x < 0)
	{
		pos.x = 448;
	}
	if (pos.x > 448)
	{
		pos.x = 0;
	}
	mOwner->SetPosition(pos);
}
void WrappingMove::SetDirection(Vector2 dir)
{
	mDirection = dir;
}