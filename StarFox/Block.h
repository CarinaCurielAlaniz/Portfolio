#include "Actor.h"
class Block : public Actor
{
public:
	Block(class Game* game, size_t idx);
	~Block();
	class CollisionComponent* GetCol() { return mCollision; }
	void OnUpdate(float deltaTime) override;
	void SetExplode(bool isExplode) { mIsExplode = isExplode; }
	bool GetExplode() const { return mIsExplode; }
	void RecurssiveExplode(Vector3 pos);

private:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class CollisionComponent* mCollision;
	bool mIsExplode = false;
};