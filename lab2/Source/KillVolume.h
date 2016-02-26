#pragma once
#include "Actor.h"
#include "BoxComponent.h"

class KillVolume : public Actor
{
	DECL_ACTOR(KillVolume, Actor);
public:
	KillVolume(Game& game);

	void BeginTouch(Actor& other) override;
private:
	BoxComponentPtr mBox;
};

DECL_PTR(KillVolume);
