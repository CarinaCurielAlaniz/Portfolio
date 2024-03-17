#include "Actor.h"
class Bullet : public Actor
{
public:
	Bullet(class Game* game);
	~Bullet();
	class CollisionComponent* GetCol() { return mCollision; }
	void OnUpdate(float deltaTime) override;
	void SetExplode(bool isExplode) { mIsExplode = isExplode; }

private:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class CollisionComponent* mCollision;
	class MoveComponent* mMove;
	bool mIsExplode = false;
	float mLifetime;
};