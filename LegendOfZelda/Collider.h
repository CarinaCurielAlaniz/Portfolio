#include "Actor.h"
class Collider : public Actor
{
public:
	Collider(class Game* game, float width, float height, Vector2 positio);
	class CollisionComponent* GetCol() { return mCollision; }
	void OnUpdate(float deltaTime) override;

private:
	class Game* mGame;
	class CollisionComponent* mCollision;
};