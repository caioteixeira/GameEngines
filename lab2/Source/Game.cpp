#include "ITPEnginePCH.h"
#include <SDL/SDL_mixer.h>
#include "Player.h"

Game::Game()
	:mRenderer(*this)
	,mAssetCache(*this, "Assets/")
	,mShouldQuit(false)
{

}

Game::~Game()
{
	mAssetCache.Clear();
	mWorld.RemoveAllActors();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

bool Game::Init()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Failed to initialize SDL.");
		return false;
	}

	// Initialize Renderer
	if (!mRenderer.Init(1024, 768))
	{
		SDL_Log("Failed to initialize renderer.");
		return false;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
	{
		SDL_Log("Failed to initialize SDL_mixer.");
		return false;
	}

	// Initialize SDL_ttf
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	// Initialize RNG
	Random::Init();

	// Add input mappings
	AddInputMappings();

	// Start frame timer
	mTimer.Start();

	// Run any code at game start
	StartGame();

	return true;
}

void Game::RunLoop()
{
	while (!mShouldQuit)
	{
		ProcessInput();
		Tick();
		GenerateOutput();
	}
}

void Game::Quit()
{
	mShouldQuit = true;
}

void Game::StartGame()
{
	// Setup camera
	auto cameraActor = Actor::Spawn(*this);
	auto cameraC = CameraComponent::Create(*cameraActor);
	cameraC->SetHorizontalDist(300.0f, 300.0f);
	cameraC->SetVerticalDist(0.0f, 0.0f);
	cameraC->SetTargetOffset(0.0f);
	cameraC->SetSpringConstant(100.0f);
	cameraC->SnapToIdealPosition();

	// Debug code for lab 2
	auto ship = Actor::Spawn(*this);
	ship->SetRotation(Quaternion(Vector3::UnitY, Math::ToRadians(-45.0f)));
	auto shipMC = MeshComponent::Create(*ship);
	shipMC->SetMesh(mAssetCache.Load<Mesh>("Meshes/PlayerShip.itpmesh2"));
	auto arrow = Actor::Spawn(*this);
	arrow->SetPosition(Vector3(0.0f, -200.0f, 100.0f));
	arrow->SetScale(5.0f);
	auto arrowMC = MeshComponent::Create(*arrow);
	arrowMC->SetMesh(mAssetCache.Load<Mesh>("Meshes/Arrow.itpmesh2"));
	auto sprite = Actor::Spawn(*this);
	sprite->SetScale(0.25f);
	sprite->SetPosition(Vector3(400.0f, -100.0f, 0.0f));
	sprite->SetRotation(Quaternion(Vector3::UnitZ, Math::ToRadians(90.0f)));
	auto spriteC = SpriteComponent::Create(*sprite);
	spriteC->SetTexture(mAssetCache.Load<Texture>("Textures/Cube.png"));
}

void Game::ProcessInput()
{
	// Poll events from SDL
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mShouldQuit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.repeat == 0)
			{
				HandleKeyPressed(event.key.keysym.sym);
			}
			break;
		case SDL_KEYUP:
			HandleKeyReleased(event.key.keysym.sym);
			break;
		default:
			// Ignore other events
			break;
		}
	}
}

void Game::HandleKeyPressed(int key)
{
	mInput.HandleKeyPressed(key);
}

void Game::HandleKeyReleased(int key)
{
	mInput.HandleKeyReleased(key);
}

void Game::Tick()
{
	// Lock @ 60 FPS
	float deltaTime = mTimer.GetFrameTime(0.016666f);

	mGameTimers.Tick(deltaTime);

	// Update game world
	mWorld.Tick(deltaTime);

	// Update physics world
	mPhysWorld.Tick(deltaTime);
}

void Game::GenerateOutput()
{
	mRenderer.RenderFrame();
}

void Game::AddInputMappings()
{
	// Parse input mappings
	mInput.ParseMappings("Config/Mappings.itpconfig");

	// Bind our quit function to "Quit" on release
	mInput.BindAction("Quit", IE_Released, this, &Game::Quit);
}
