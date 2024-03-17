#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector3::Zero)
, mScale(1.0f)
, mRotation(0.0f)
, mRollRotation(0.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		delete mComponents[i];
	}
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	if (mState == ActorState::Active)
	{
		for (size_t i = 0; i < mComponents.size(); i++)
		{
			mComponents[i]->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}
	CalcWorldTransform();
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	if (mState == ActorState::Active)
	{
		for (size_t i = 0; i < mComponents.size(); i++)
		{
			mComponents[i]->ProcessInput(keyState);
		}
		OnProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

void Actor::CalcWorldTransform()
{
	Matrix4 scale = Matrix4::CreateScale(mScale);
	Matrix4 rotation = Matrix4::CreateRotationZ(mRotation);
	Matrix4 translation = Matrix4::CreateTranslation(mPosition);
	Matrix4 barrelRoll = Matrix4::CreateRotationX(mRollRotation);
	mWorldTransform = scale * rotation * barrelRoll * translation;
}