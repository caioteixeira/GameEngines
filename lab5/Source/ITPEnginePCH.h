#pragma once

// Windows/Directx
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#endif

// STL
#include <fstream>
#include <sstream>
#include <string>

// Core
#include "Math.h"
#include "SimdMath.h"
#include "DbgAssert.h"
#include "Delegate.h"
#include "Random.h"
#include "PoolAlloc.h"
#include "BoneTransform.h"
#include "MatrixPalette.h"
#include "CollisionHelpers.h"
#include <rapidjson/document.h>
#include "Object.h"
#include "LevelLoader.h"

// Assets
#include "AssetCache.h"
#include "Asset.h"
#include "Animation.h"
#include "Font.h"
#include "Mesh.h"
#include "Shader.h"
#include "Skeleton.h"
#include "Sound.h"
#include "Texture.h"

#include "Object.h"

// Components
#include "Component.h"
#include "AudioComponent.h"
#include "CameraComponent.h"
#include "CharacterMoveComponent.h"
#include "CollisionComponent.h"
#include "BoxComponent.h"
#include "DrawComponent.h"
#include "FontComponent.h"
#include "InputComponent.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "PointLightComponent.h"
#include "SkeletalMeshComponent.h"
#include "SphereComponent.h"
#include "SpriteComponent.h"

#include "Actor.h"

#include "KillVolume.h"

#include "Player.h"

#include "Game.h"
#include "Renderer.h"
