// Player.h
// Player actor

#pragma once
#include "Actor.h"
#include "Texture.h"
#include "CharacterMoveComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "Skeleton.h"

class Player : public Actor
{
	DECL_ACTOR(Player, Actor);
public:
	Player(Game& game);

	void BeginPlay() override;
	void Tick(float deltaTime) override;
	void EndPlay() override;

	void BeginTouch(Actor& other) override;

	void OnRecenter();

	void OnRespawn();

	void SetProperties(const rapidjson::Value& properties) override;
private:
	Vector3 mRespawnPosition;

	// Need pointer to my sprite component to change texture
	CharacterMoveComponentPtr mCharMove;
	SkeletalMeshComponentPtr mMesh;
	CameraComponentPtr mCamera;

	SkeletonPtr mSkeleton;
};

DECL_PTR(Player);
