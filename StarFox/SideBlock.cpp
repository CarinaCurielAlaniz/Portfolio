#include "SideBlock.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Player.h"
#include "PlayerMove.h"
SideBlock::SideBlock(class Game* game, size_t idx)
: Actor(game)
, mMeshComp(new MeshComponent(this))
, mCollision(new CollisionComponent(this))
{
	mGame = game;
	SetScale(500.0f);
	mMeshComp->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mMeshComp->SetTextureIndex(idx);
}

void SideBlock::OnUpdate(float deltaTime)
{
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector3 blockPos = GetPosition();
	if (playerPos.x - blockPos.x >= 2000.0f)
	{
		this->SetState(ActorState::Destroy);
		//L_Log("DESTROY at side block at.....%f and ship pos.....%f", blockPos.x, playerPos.x);
	}
}