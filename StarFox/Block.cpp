#include "Block.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Player.h"
#include "PlayerMove.h"
Block::Block(class Game* game, size_t idx)
: Actor(game)
, mMeshComp(new MeshComponent(this))
, mCollision(new CollisionComponent(this))
{
	mGame = game;
	SetScale(25.0f);
	mMeshComp->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mMeshComp->SetTextureIndex(idx);
	mCollision->SetSize(1, 1, 1);
	mGame->AddBlock(this);
}
Block::~Block()
{
	mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime)
{
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector3 blockPos = GetPosition();
	if (playerPos.x - blockPos.x >= 2000.0f)
	{
		this->SetState(ActorState::Destroy);
		//L_Log("DESTROY at block position.....%f and ship pos.....%f", blockPos.x, playerPos.x);
	}
}

void Block::RecurssiveExplode(Vector3 pos)
{
	std::vector<Block*>& blocks = mGame->GetBlock();
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->GetState() != ActorState::Destroy &&
			Vector3::Distance(pos, blocks[i]->GetPosition()) <= 50.0f)
		{
			blocks[i]->SetState(ActorState::Destroy);
			if (blocks[i]->GetExplode())
			{
				RecurssiveExplode(blocks[i]->GetPosition());
			}
		}
	}
}