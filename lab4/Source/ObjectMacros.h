// ObjectMacros.h
// Helper mactors that define common functionality
// needed by components, actors, and assets
// Edit this file at your own peril!

#pragma once

// Set this to 1 if you want components to use the pool allocator
#define COMPONENTS_USE_POOLS 1

#if !COMPONENTS_USE_POOLS
	// Use this as DECL_COMPONENT(SelfClass, SuperClass)
	#define DECL_COMPONENT(d,s) typedef s Super; \
		std::shared_ptr<d> ThisPtr() \
		{ \
			return std::static_pointer_cast<d>(shared_from_this()); \
		} \
		static const TypeInfo sType; \
		public: \
		static const TypeInfo* StaticType() { return &sType; } \
		const TypeInfo* GetType() const override { return &sType; } \
		static std::shared_ptr<d> Create(class Actor& actor, \
			UpdateType update = PostTick); \
		static std::shared_ptr<d> CreateWithProperties(class Actor& actor, \
			UpdateType update, const rapidjson::Value& properties); \
		private:
#else // COMPONENTS_USE_POOL == 1
	// Use this as DECL_COMPONENT(SelfClass, SuperClass)
	#define DECL_COMPONENT(d,s) typedef s Super; \
		std::shared_ptr<d> ThisPtr() \
		{ \
			return std::static_pointer_cast<d>(shared_from_this()); \
		} \
		static const TypeInfo sType; \
		public: \
		static const TypeInfo* StaticType() { return &sType; } \
		const TypeInfo* GetType() const override { return &sType; } \
		static std::shared_ptr<d> Create(class Actor& actor, \
			UpdateType update = PostTick); \
		static std::shared_ptr<d> CreateWithProperties(class Actor& actor, \
			UpdateType update, const rapidjson::Value& properties); \
		static void* operator new(size_t size); \
		static void operator delete(void* ptr); \
		private:
#endif

#if !COMPONENTS_USE_POOLS
	// Use this as IMPL_COMPONENT(SelfClass, SuperClass, MaxCount)
	#define IMPL_COMPONENT(d,s,c) \
		const TypeInfo d::sType(s::StaticType()); \
		std::shared_ptr<d> d::Create(Actor& actor, UpdateType update) \
		{\
			std::shared_ptr<d> ptr = std::make_shared<d>(actor); \
			ptr->mOwner.AddComponent(ptr, update); \
			return ptr; \
		}\
		std::shared_ptr<d> d::CreateWithProperties(Actor& actor, UpdateType update, \
			const rapidjson::Value& properties) \
		{\
			std::shared_ptr<d> ptr = std::make_shared<d>(actor); \
			ptr->mOwner.AddComponent(ptr, update); \
			ptr->SetProperties(properties); \
			return ptr; \
		}
#else // COMPONENTS_USE_POOL == 1
	// Use this as IMPL_COMPONENT(SelfClass, SuperClass, MaxCount)
	#define IMPL_COMPONENT(d,s,c) \
		const TypeInfo d::sType(s::StaticType()); \
		std::shared_ptr<d> d::Create(Actor& actor, UpdateType update) \
		{\
			std::shared_ptr<d> ptr = std::make_shared<d>(actor); \
			ptr->mOwner.AddComponent(ptr, update); \
			return ptr; \
		}\
		std::shared_ptr<d> d::CreateWithProperties(Actor& actor, UpdateType update, \
			const rapidjson::Value& properties) \
		{\
			std::shared_ptr<d> ptr = std::make_shared<d>(actor); \
			ptr->mOwner.AddComponent(ptr, update); \
			ptr->SetProperties(properties); \
			return ptr; \
		}\
		static PoolAllocator<sizeof(d), c> sPoolAllocator; \
		void* d::operator new(size_t size) \
		{\
			return sPoolAllocator.Allocate(size); \
		}\
		void d::operator delete(void* ptr) \
		{\
			sPoolAllocator.Free(ptr);\
		}
#endif

// Use this as DECL_ACTOR(SelfClass, SuperClass)
#define DECL_ACTOR(d,s) typedef s Super; \
	std::shared_ptr<d> ThisPtr() \
	{ \
		return std::static_pointer_cast<d>(shared_from_this()); \
	} \
	static const TypeInfo sType; \
	public: \
	static const TypeInfo* StaticType() { return &sType; } \
	const TypeInfo* GetType() const override { return &sType; } \
	static std::shared_ptr<d> Spawn(class Game& game); \
	static std::shared_ptr<d> SpawnAttached(Actor& parent); \
	static std::shared_ptr<d> SpawnWithProperties(class Game& game, const rapidjson::Value& properties); \
	private: \

// Use this as IMPL_ACTOR(SelfClass, SuperClass)
#define IMPL_ACTOR(d,s) \
	const TypeInfo d::sType(s::StaticType()); \
	std::shared_ptr<d> d::Spawn(Game& game) \
	{\
		std::shared_ptr<d> ptr = std::make_shared<d>(game); \
		game.GetWorld().AddActor(ptr); \
		ptr->BeginPlay(); \
		return ptr; \
	}\
	std::shared_ptr<d> d::SpawnAttached(Actor& parent) \
	{\
		std::shared_ptr<d> ptr = std::make_shared<d>(parent.GetGame()); \
		parent.AddChild(ptr); \
		ptr->BeginPlay(); \
		return ptr; \
	}\
	std::shared_ptr<d> d::SpawnWithProperties(class Game& game, const rapidjson::Value& properties) \
	{\
		std::shared_ptr<d> ptr = std::make_shared<d>(game); \
		game.GetWorld().AddActor(ptr); \
		ptr->SetProperties(properties); \
		return ptr; \
	}\


// Use this as DECL_ASSET(SelfClass, SuperClass)
#define DECL_ASSET(d,s) typedef s Super; \
	std::shared_ptr<d> ThisPtr() \
	{ \
		return std::static_pointer_cast<d>(shared_from_this()); \
	} \
	public: \
	static std::shared_ptr<d> StaticLoad(const char* file, class AssetCache* cache, Game& game) \
	{ \
		std::shared_ptr<d> ptr = std::make_shared<d>(game); \
		if (!ptr->Load(file, cache)) { return nullptr; } \
		return ptr; \
	} \
	private: \

// Use this as DECL_PTR(Class)
#define DECL_PTR(o) typedef std::shared_ptr<o> o##Ptr;
