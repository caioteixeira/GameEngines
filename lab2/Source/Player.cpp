#include "ITPEnginePCH.h"
#include "Player.h"

IMPL_ACTOR(Player, Actor);

Player::Player(Game& game)
	:Actor(game)
{
	mCharMove = CharacterMoveComponent::Create(*this, Component::PreTick);

	mMesh = SkeletalMeshComponent::Create(*this);

	mCamera = CameraComponent::Create(*this);
	mCamera->SetMoveComponent(mCharMove);

	// Setup animations for character move component
	mCharMove->SetOwnerMesh(mMesh);
}

void Player::BeginPlay()
{
	Super::BeginPlay();

	mGame.GetInput().BindAction("Recenter", IE_Pressed, this, &Player::OnRecenter);
	// Setup our input component
	mCharMove->BindLinearAxis("Forward");
	mCharMove->BindHorizontalAxis("Left");
	mCharMove->BindJump("Jump");

	mCamera->SnapToIdealPosition();
}

void Player::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

}

void Player::EndPlay()
{
}

void Player::BeginTouch(Actor& other)
{

}

void Player::OnRecenter()
{
	mCamera->SnapToIdealPosition();
}

void Player::OnRespawn()
{
	SetPosition(mRespawnPosition);
	OnRecenter();
}

void Player::SetProperties(const rapidjson::Value& properties)
{
	GetVectorFromJSON(properties, "respawnPosition", mRespawnPosition);
}
