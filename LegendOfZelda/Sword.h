#include "Actor.h"
class Sword : public Actor
{
public:
	Sword(class Game* game, Vector2 position);
	class CollisionComponent* GetCol() { return mCollision; }

private:
	class Game* mGame;
	class CollisionComponent* mCollision;
};