#include "Actor.h"
class SideBlock : public Actor
{
public:
	SideBlock(class Game* game, size_t idx);
	class CollisionComponent* GetCol() { return mCollision; }
	void OnUpdate(float deltaTime) override;

private:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class CollisionComponent* mCollision;
};