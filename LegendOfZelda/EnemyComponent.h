#pragma once
#include "Component.h"
#include <functional>

class EnemyComponent : public Component
{
public:
	EnemyComponent(class Actor* owner);
	~EnemyComponent();
	class CollisionComponent* GetCol();
	int GetHitPoints() const { return mHitPoints; }
	void Update(float deltaTime) override;
	void SetHitPoints(int points);
	void TakeDamage();
	void SetOnDeath(std::function<void()> onDeath);
	void SetOnDamage(std::function<void()> onDamage);

protected:
	class CollisionComponent* mCol;
	class Actor* mOwner;
	int mHitPoints;
	float mLifeTime;
	bool mTookDamage;
	std::function<void()> mOnDamage;
	std::function<void()> mOnDeath;
};
