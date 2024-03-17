#include "Actor.h"
class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCol() { return mCollision; }
	void OnUpdate(float deltaTime) override;
	int GetShield() const { return mShield; }
	void SetShield(int x) { mShield = x; }
	class HUD* GetHud() const { return mHUD; }

private:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class CollisionComponent* mCollision;
	class PlayerMove* mPlayerMove;
	int mShield = 3;
	class HUD* mHUD;
};