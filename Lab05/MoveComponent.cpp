#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
: Component(owner, 50)
, mAngularSpeed(0.0f)
, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	float rot = mOwner->GetRotation() + mAngularSpeed * deltaTime;
	mOwner->SetRotation(rot);
	Vector2 pos = mOwner->GetPosition();
	Vector2 forwardVec = mOwner->GetForward();
	Vector2 velocity = forwardVec * mForwardSpeed;
	mOwner->SetPosition(pos + velocity * deltaTime);
}