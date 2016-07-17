#include "ITPEnginePCH.h"

World::World()
{

}

World::~World()
{

}

void World::AddActor(ActorPtr actor)
{
	mActors.emplace(actor);
}

void World::Tick(float deltaTime)
{
	// Tick a copy of the actor pointers, in case any actor adds more actors
	auto actors = mActors;
	for (auto& actor : actors)
	{
		actor->TickInternal(deltaTime);
	}

	// Remove any actors that are dead
	auto iter = mActors.begin();
	while (iter != mActors.end())
	{
		if (!(*iter)->IsAlive())
		{
			(*iter)->EndPlay();
			iter = mActors.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void World::RemoveAllActors()
{
	for (auto& actor : mActors)
	{
		actor->EndPlay();
	}
	mActors.clear();
}
