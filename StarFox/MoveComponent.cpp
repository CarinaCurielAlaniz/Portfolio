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
	Vector3 pos = mOwner->GetPosition();
	Vector3 forwardVec = mOwner->GetForward();
	Vector3 velocity = forwardVec * mForwardSpeed;
	mOwner->SetPosition(pos + velocity * deltaTime);
}