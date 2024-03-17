#include "Actor.h"
class Player : public Actor
{
public:
	Player(class Game* game);

	class AnimatedSprite* GetSprite() { return mAnimSprite; }
	class CollisionComponent* GetCol() { return mCollision; }
	class PlayerMove* GetMove() { return mMove; }
	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState) override;

private:
	class Game* mGame;
	class AnimatedSprite* mAnimSprite;
	class CollisionComponent* mCollision;
	class PlayerMove* mMove;
};