#include "ITPEnginePCH.h"
#include <SDL/SDL_mixer.h>
#include "Player.h"
#include <thread>

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
	//Generate first frame data
	ProcessInput();
	Tick();
	mRenderer.ExtractComponents(mRenderQueue);

	//Initialize render thread
	std::thread renderThread(&Game::RenderLoop, this);
	
	//Update frames
	UpdateLoop();

	//Join render thread before quit
	renderThread.join();
}

void Game::RenderLoop()
{
	while (!mShouldQuit)
	{
		{
			std::lock_guard<std::mutex> lock(mRenderMutex);
			if (mRenderQueue.empty())
			{
				continue;
			}

			GenerateOutput();
		}
	}
}

void Game::UpdateLoop()
{
	while (!mShouldQuit)
	{
		ProcessInput();
		Tick();
		mRenderer.ExtractComponents(mUpdateQueue);

		//Extract render data
		{ //Swap Queues
			std::lock_guard<std::mutex> lock(mRenderMutex);
			swap(mRenderQueue, mUpdateQueue);
		}
		
	}
}

void Game::Quit()
{
	mShouldQuit = true;
}

void Game::StartGame()
{
	LevelLoader loader(*this);
	loader.Load("Assets/Levels/lab5.itplevel");
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
	mRenderer.RenderFrame(mRenderQueue);
}



void Game::AddInputMappings()
{
	// Parse input mappings
	mInput.ParseMappings("Config/Mappings.itpconfig");

	// Bind our quit function to "Quit" on release
	mInput.BindAction("Quit", IE_Released, this, &Game::Quit);
}
