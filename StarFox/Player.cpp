#include "Player.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "PlayerMove.h"
#include "HUD.h"

Player::Player(class Game* game)
: Actor(game)
, mMeshComp(new MeshComponent(this))
, mCollision(new CollisionComponent(this))
, mPlayerMove(new PlayerMove(this))
, mHUD(new HUD(this))
{
	mGame = game;
	SetScale(2.0f);
	mMeshComp->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arwing.gpmesh"));
	mCollision->SetSize(40.0f, 25.0f, 15.0f);
}

void Player::OnUpdate(float deltaTime)
{
}
