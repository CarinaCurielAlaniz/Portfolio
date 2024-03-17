#include "Actor.h"
class Soldier : public Actor
{
public:
	Soldier(class Game* game, Vector2 position, class PathNode* start, class PathNode* end);
	class CollisionComponent* GetCol() { return mCollision; }

private:
	class Game* mGame;
	class CollisionComponent* mCollision;
	class AnimatedSprite* mAnim;
	class SoldierAI* mAI;
	class EnemyComponent* mEnemyComp;
};