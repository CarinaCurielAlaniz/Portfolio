#include "Bullet.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Player.h"
#include "Block.h"
#include "PlayerMove.h"
Bullet::Bullet(class Game* game)
: Actor(game)
, mMeshComp(new MeshComponent(this))
, mCollision(new CollisionComponent(this))
, mMove(new MoveComponent(this))
{
	mGame = game;
	SetScale(5.0f);
	mMeshComp->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
	SetPosition(game->GetPlayer()->GetPosition());
	mCollision->SetSize(10, 10, 10);
	mMove->SetForwardSpeed(900.0f);
	mLifetime = 1.0f;
}
Bullet::~Bullet()
{
}

void Bullet::OnUpdate(float deltaTime)
{
	float mult = mGame->GetPlayer()->GetComponent<PlayerMove>()->GetMultSpeed();
	mMove->SetForwardSpeed(900.0f * mult);
	mLifetime -= deltaTime;
	if (mLifetime <= 0.0f)
	{
		SetState(ActorState::Destroy);
	}
	std::vector<Block*>& blockCop = mGame->GetBlock();
	//go thru b;ocks
	bool doesCollide = false;
	bool alreadyPlayed = false;
	for (int i = 0; i < blockCop.size(); i++)
	{
		doesCollide = mCollision->Intersect(blockCop[i]->GetCol());
		//if they collide destory laser
		if (doesCollide)
		{
			SetState(ActorState::Destroy);
			//if block can explode explode it
			if (blockCop[i]->GetExplode())
			{
				blockCop[i]->SetState(ActorState::Destroy);
				blockCop[i]->RecurssiveExplode(blockCop[i]->GetPosition());
				//explode radius
			}
		}
		if (doesCollide && !alreadyPlayed)
		{
			alreadyPlayed = true;
			mGame->GetAudio()->PlaySound("BlockExplode.wav");
		}
	}

	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector3 blockPos = GetPosition();
	if (playerPos.x - blockPos.x >= 2000.0f)
	{
		SetState(ActorState::Destroy);
	}
}