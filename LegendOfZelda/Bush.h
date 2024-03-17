#include "Actor.h"
class Bush : public Actor
{
public:
	Bush(class Game* game, Vector2 position);
	class CollisionComponent* GetCol() { return mCollision; }
	void OnUpdate(float deltaTime) override;

private:
	class Game* mGame;
	class CollisionComponent* mCollision;
	class SpriteComponent* mSprite;
	class EnemyComponent* mEnemyComp;
	class PathFinder* mPathF;
};