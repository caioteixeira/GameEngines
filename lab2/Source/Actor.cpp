#include "ITPEnginePCH.h"

IMPL_ACTOR(Actor, Object);

Actor::Actor(Game& game)
	:mGame(game)
	,mParent(nullptr)
	,mScale(1.0f)
	,mIsAlive(true)
	,mIsPaused(false)
{

}

Actor::~Actor()
{
	mGame.GetGameTimers().ClearAllTimers(this);
	RemoveAllChildren();
	RemoveAllComponents();
}

void Actor::BeginPlay()
{

}

void Actor::Tick(float deltaTime)
{

}

void Actor::EndPlay()
{

}

void Actor::BeginTouch(Actor& other)
{

}

void Actor::AddComponent(ComponentPtr component, Component::UpdateType update)
{
	component->Register();
	if (update == Component::PostTick)
	{
		mPostTickComponents.emplace(component);
	}
	else
	{
		mPreTickComponents.emplace(component);
	}
}

void Actor::RemoveComponent(ComponentPtr component)
{
	component->Unregister();

	// This may be pre-tick or post-tick
	auto iter = mPreTickComponents.find(component);
	if (iter != mPreTickComponents.end())
	{
		mPreTickComponents.erase(component);
	}

	iter = mPostTickComponents.find(component);
	if (iter != mPreTickComponents.end())
	{
		mPostTickComponents.erase(component);
	}
}

void Actor::AddChild(ActorPtr child)
{
	mChildren.emplace(child);
	child->mParent = this;
	// Force the child to compute their transform matrix
	child->ComputeWorldTransform();
}

void Actor::RemoveChild(ActorPtr child)
{
	auto iter = mChildren.find(child);
	if (iter != mChildren.end())
	{
		(*iter)->EndPlay();
		mChildren.erase(iter);
	}
	child->mParent = nullptr;
}

Vector3 Actor::GetForward() const
{
	// Following Unreal coordinate system so X is forward
	return mWorldTransform.GetXAxis();
}

ComponentPtr Actor::GetComponentFromType(const TypeInfo* type)
{
	// Check the pre-tick ones first
	for (auto& comp : mPreTickComponents)
	{
		if (type->IsExactly(comp->GetType()))
		{
			return comp;
		}
	}

	// Now post-tick
	for (auto& comp : mPostTickComponents)
	{
		if (type->IsExactly(comp->GetType()))
		{
			return comp;
		}
	}

	// Didn't find anything so give up
	return nullptr;
}

void Actor::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	// TODO: Lab 3
}

void Actor::ComputeWorldTransform()
{
	// NOTE: Computing this recursively is not the most efficient
	// but it's the simplest to write!
	mWorldTransform = Matrix4::CreateScale(mScale) *
		Matrix4::CreateFromQuaternion(mRotation) *
		Matrix4::CreateTranslation(mPosition);

	// No parent is the base case
	if (mParent)
	{
		// My transform * Parent's transform
		mWorldTransform *= mParent->GetWorldTransform();
	}

	// Tell my children to recompute
	for (auto& child : mChildren)
	{
		child->ComputeWorldTransform();
	}

	// Notify my components that my transform has updated
	for (auto& comp : mPreTickComponents)
	{
		comp->OnUpdatedTransform();
	}

	for (auto& comp : mPostTickComponents)
	{
		comp->OnUpdatedTransform();
	}
}

void Actor::TickInternal(float deltaTime)
{
	if (!mIsPaused)
	{
		// Tick pre-tick components
		for (auto& comp : mPreTickComponents)
		{
			comp->Tick(deltaTime);
		}

		// Tick myself
		Tick(deltaTime);

		// Tick post-tick components
		for (auto& comp : mPostTickComponents)
		{
			comp->Tick(deltaTime);
		}

		// Tick any children
		for (auto& child : mChildren)
		{
			child->TickInternal(deltaTime);
		}
	}
}

void Actor::RemoveAllComponents()
{
	// Unregister everything first
	for (auto& comp : mPreTickComponents)
	{
		comp->Unregister();
	}

	for (auto& comp : mPostTickComponents)
	{
		comp->Unregister();
	}

	mPreTickComponents.clear();
	mPostTickComponents.clear();
}

void Actor::RemoveAllChildren()
{
	for (auto& child : mChildren)
	{
		child->EndPlay();
		child->mParent = nullptr;
	}

	mChildren.clear();
}
