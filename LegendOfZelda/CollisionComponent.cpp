#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	bool noIntersection = GetMax().x < other->GetMin().x || other->GetMax().x < GetMin().x ||
						  GetMax().y < other->GetMin().y || other->GetMax().y < GetMin().y;
	return !noIntersection;
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 pos = mOwner->GetPosition();
	Vector2 min;
	min.x = pos.x - (mWidth * mOwner->GetScale()) / 2.0f;
	min.y = pos.y - (mHeight * mOwner->GetScale()) / 2.0f;
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	Vector2 pos = mOwner->GetPosition();
	Vector2 max;
	max.x = pos.x + (mWidth * mOwner->GetScale()) / 2.0f;
	max.y = pos.y + (mHeight * mOwner->GetScale()) / 2.0f;
	return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset) const
{
	float distances[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	if (Intersect(other))
	{
		//botDist
		distances[0] = Math::Abs(((GetMin()).y) - (other->GetMax().y));
		//topDist
		distances[1] = Math::Abs((other->GetMin().y) - ((GetMax()).y));
		//leftDist
		distances[2] = Math::Abs((GetMax().x) - ((other->GetMin()).x));
		//rightDist
		distances[3] = Math::Abs(((other->GetMax()).x) - (GetMin().x));

		//findMin
		float min = distances[0];
		int minIndex = 0;
		for (int i = 0; i < 4; i++)
		{
			if (distances[i] < min)
			{
				min = distances[i];
				minIndex = i;
			}
		}
		switch (minIndex)
		{
		case 0: // botDist
			offset = Vector2(0.0f, min);
			return CollSide::Bottom;
		case 1: // topDist
			offset = Vector2(0.0f, -min);
			return CollSide::Top;
		case 2: // leftDist
			offset = Vector2(-min, 0.0f);
			return CollSide::Left;
		case 3: // rightDist
			offset = Vector2(min, 0.0f);
			return CollSide::Right;
		}
	}
	offset = Vector2::Zero;
	return CollSide::None;
}
