#include "ITPEnginePCH.h"

IMPL_ACTOR(KillVolume, Actor);

KillVolume::KillVolume(Game& game)
	:Actor(game)
{
	mBox = BoxComponent::Create(*this);
	Collision::AxisAlignedBox box;
	box.mMin = Vector3(-0.5f, -0.5f, -0.5f);
	box.mMax = Vector3(0.5f, 0.5f, 0.5f);
	mBox->BoxFromBox(box);
}

void KillVolume::BeginTouch(Actor& other)
{
	if (IsA<Player>(other))
	{
		auto& player = Cast<Player>(other);
		player.OnRespawn();
	}
}
