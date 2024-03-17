#include "EnemyComponent.h"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"

EnemyComponent::EnemyComponent(class Actor* owner)
: Component(owner, 50)
{
	mOwner = owner;
	mCol = mOwner->GetComponent<CollisionComponent>();
	owner->GetGame()->AddEnemies(this);
	mLifeTime = 0.25f;
	mTookDamage = false;
	mHitPoints = 0;
}
EnemyComponent::~EnemyComponent()
{
	mOwner->GetGame()->RemoveEnemies(this);
}
void EnemyComponent::Update(float deltaTime)
{
	if (mTookDamage)
	{
		mLifeTime -= deltaTime;
		if (mLifeTime <= 0.0f)
		{
			mTookDamage = false;
		}
	}
}

class CollisionComponent* EnemyComponent::GetCol()
{
	return mCol;
}
void EnemyComponent::SetHitPoints(int points)
{
	mHitPoints = points;
}

void EnemyComponent::TakeDamage()
{
	if (!mTookDamage)
	{
		mTookDamage = true;
		mHitPoints--;
		if (mHitPoints <= 0)
		{
			if (mOnDeath)
			{
				mOnDeath();
			}
			mOwner->SetState(ActorState::Destroy);
		}
		else
		{
			if (mOnDamage)
			{
				mOnDamage();
			}
		}
	}
}

void EnemyComponent::SetOnDeath(std::function<void()> onDeath)
{
	mOnDeath = onDeath;
}
void EnemyComponent::SetOnDamage(std::function<void()> onDamage)
{
	mOnDamage = onDamage;
}