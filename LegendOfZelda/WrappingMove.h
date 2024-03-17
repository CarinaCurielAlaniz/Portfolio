#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

class WrappingMove : public MoveComponent
{
public:
	WrappingMove(class Actor* owner);
	void Update(float deltaTime) override;
	void SetDirection(Vector2 dir);

private:
	Vector2 mDirection;
};