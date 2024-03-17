#include "Effect.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "EnemyComponent.h"
#include "AudioSystem.h"

Effect::Effect(class Game* game, Vector2 position, std::string animName, std::string soundName)
: Actor(game)
{
	SetPosition(position);
	AnimatedSprite* anim = new AnimatedSprite(this);
	anim->LoadAnimations("Assets/Effects");
	anim->SetAnimation(animName);
	mLifeTime = anim->GetAnimDuration(animName);
	game->GetAudio()->PlaySound(soundName);
}
void Effect::OnUpdate(float deltaTime)
{
	mLifeTime -= deltaTime;
	if (mLifeTime <= 0.0f)
	{
		SetState(ActorState::Destroy);
	}
}