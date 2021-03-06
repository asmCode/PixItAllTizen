#include "Game.h"
#include "ClassContainer.h"
#include "Utils.h"
#include "IGameState.h"
#include "SplashScreen.h"
#include "MainMenuGameState.h"
#include "SelectLevelGameState.h"
#include "PlayGameState.h"
#include "ImagesCollection.h"
#include "Control.h"
#include "OptionsPanel.h"
#include "SoundManager.h"
#include "Leaderboard.h"
#include "Fade2.h"
#include "MessageBoxManager.h"
#include "IGameCommunity.h"
#include "GameCommunityFactory.h"
#include "IGameCenterViewProxy.h"
#include "Options.h"
#include "ScreenKeyboard.h"
#include "AchievementsManager.h"
#include "PlayerData.h"
#include "Environment.h"
#include "ISystemUtils.h"
#include <Utils/MemUtils.h>
#include <Utils/Math/Matrix.h>
#include <Utils/Log.h>
#include <Content/DemoContent.h>
#include <GraphicsLibrary/FontRenderer.h>
#include <GraphicsLibrary/SpriteBatch.h>
#include <GraphicsLibrary/Texture.h>
#include <GraphicsLibrary/OpenglPort.h>
#include <math.h>

#include <FIo.h>

using namespace Tizen::Graphics::Opengl;

template<typename T> extern T* GenericSingleton<T>::instance;

Game::Game(IGameCenterViewProxy *gameCenterViewProxy,
		   SplashScreen *splashScreen)
{
	spriteBatch = NULL;
	optionsPanel = NULL;
	soundMng = NULL;
	nextGameState = NULL;
	isWaitingForFade = false;
	imagesCollection = NULL;
	gc = NULL;
	
	this->gameCenterViewProxy = gameCenterViewProxy;
	this->splashScreen = splashScreen;
	this->gameState = NULL;
}

Game::~Game()
{
	Deinitialize();
}

bool Game::Initialize(const std::string &basePath,
					  const std::string &docPath,
					  ISystemUtils *systemUtils)
{
//	SplashScreen *splashScreen = new SplashScreen(NULL, basePath + "splash.png");
//	if (!splashScreen->Initialize())
//	{
//		assert(false);
//	}
//	//splashScreen->Draw(0, 0);
//	gameState = splashScreen;
//
//	splashScreen->SetGame(this);
	Log::LogT("Base path: %s", basePath.c_str());
	Log::LogT("Doc path: %s", docPath.c_str());
	
	m_systemUtils = systemUtils;

	PlayerData::GetInstance()->Initialize(docPath + "player_data.xml");
	PlayerData::GetInstance()->Load();

	Options::GetInstance()->SetOptionsFile(docPath + "/options");
	if (!Options::GetInstance()->Load())
		Options::GetInstance()->Save();
	
	this->docPath = docPath;
	this->basePath = basePath;

	// images collection must be initialized before game center, cos we need to calculate current points
	imagesCollection = ImagesCollection::Load(basePath + "images/",
											  "images.def");
	if (imagesCollection == NULL)
	{
		assert(false);
		Deinitialize();
		return false;
	}
	
//	std::vector<std::string> files;
//	Path::GetAllFiles(files, basePath + "images/", "*.png");
//	std::stringstream ss;
//	std::ofstream of((docPath + "/dupa.xml").c_str());
//	for (unsigned i = 0; i < files.size(); i++)
//	{
//		Path path(files[i]);
//		std::string name = path.GetFilename();
//		ss << "<image id=\"" << name << "\" file_source=\"" << files[i] << "\" title=\"" << name << "\" fail_limit=\"5\"/>\n";
//	}
//	of.write(ss.str().c_str(), ss.str().size());
//	of.close();
	//<image id="test_small" file_source="small.png" title="Maly 1" fail_limit="3"/>
	
	// create folder for common user
	Utils::CreateDir(docPath + "/levels_stats/common_user/");
	
	// by default load common_user stats, because game center may fail
	imagesCollection->LoadLevelStats(docPath + "/levels_stats/common_user/");
	
	// ensure that desired amount of levels are unlocked
	imagesCollection->UnlockLevels();
	
	gc = GameCommunityFactory::CreateGameCenter(gameCenterViewProxy);
	ObsCast(IGameCommunityObserver, gc)->AddObserver(this);
	
	AchievementsManager::GetInstance()->SetGameCommunity(gc);
	
	ClassContainer::GetInstance()->AddClass("gc", gc);
	
	gc->AuthenticatePlayer();
		
	if (!LoadContent(basePath))
		return false;

	spriteBatch = new SpriteBatch();
	if (spriteBatch == NULL)
	{
		Deinitialize();
		return false;
	}
		
	soundMng = SoundManager::GetInstance();
//	ObsCast(IPropertyObserver, soundMng)->AddObserver(this);
//	soundMng->SetAudioBasePath(basePath + "audio/");
//	BackgroundWorker *bgWorker = [[BackgroundWorker alloc] initWithAsync:soundMng];
//	[bgWorker start];
	if (!soundMng->Initialize(basePath + "audio/"))
	{
		assert(false);
	}
	
	soundMng->SetMusicVolume(Options::GetInstance()->GetMusicVolume());
	soundMng->SetSoundVolume(Options::GetInstance()->GetSoundsVolume());
	
	ClassContainer::GetInstance()->AddClass("soundManager", soundMng);
	
	msgBoxMan = MessageBoxManager::GetInstance();
	
	optionsPanel = OptionsPanel::Create(soundMng);
	assert(optionsPanel != NULL);
	
	mainMenuGameState = new MainMenuGameState(this, optionsPanel, gameCenterViewProxy);
	mainMenuGameState->Initialize();
	mainMenuGameState->SetGameCenterButtons(false); // disable by default

	selectLevelGameState = new SelectLevelGameState(this, imagesCollection, gameCenterViewProxy);
	selectLevelGameState->Initialize();
	
	playGameState = new PlayGameState(this, imagesCollection, optionsPanel);
	if (!playGameState->Initialize())
	{
		assert(false);
	}
	
	Control::SetSpriteBatch(spriteBatch);
	
	if (soundMng != NULL)
		soundMng->PlayMusic();

	SendPlayerData();

	gameState = mainMenuGameState;
	gameState->SetFocus();

	return true;
}

void Game::Deinitialize()
{	
	MemUtils::DeleteObject(spriteBatch);
	MemUtils::DeleteObject(optionsPanel);
	MemUtils::DeleteObject(gc);
}

bool Game::LoadContent(const std::string &basePath)
{		
	DemoContent *dc = DemoContent::GetInstance();
	
	unsigned screenWidth = PixItAll::Environment::GetInstance()->GetScreenWidth();
	std::string texFolder;
	switch (screenWidth)
	{
		case 640:
			texFolder = "tex/640";
			break;
			
		case 768:
			texFolder = "tex/768";
			break;

		case 720:
			texFolder = "tex/768";
			break;

		case 320:
			texFolder = "tex/320";
			break;
	}
	
	dc ->LoadResources(basePath.c_str(),
					   texFolder.c_str());
	
	Texture *tex = dc ->Get<Texture*>("gui_01");
	assert(tex != NULL);
	tex ->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	tex = dc ->Get<Texture*>("gui_02");
	assert(tex != NULL);
	tex ->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	tex = dc ->Get<Texture*>("flood_anim");
	assert(tex != NULL);
	tex ->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	FontRenderer *defaultFont = NULL;
	FontRenderer *allan18Font = NULL;
	FontRenderer *allan30Font = NULL;
	FontRenderer *carter28Font = NULL;
	FontRenderer *carter24Font = NULL;
	
	switch (screenWidth)
	{
		case 640:
			defaultFont = FontRenderer::LoadFromFile((basePath + "fonts/komika_title_32.xml").c_str());
			Utils::LoadGuiMap(basePath + "gui/640/gui.map");
			Utils::LoadGuiProperties(basePath + "gui/640/");
			break;
			
		case 720:
			defaultFont = FontRenderer::LoadFromFile((basePath + "fonts/komika_title_32.xml").c_str());
			allan18Font = FontRenderer::LoadFromFile((basePath + "fonts/allan_18.xml").c_str());
			allan30Font = FontRenderer::LoadFromFile((basePath + "fonts/allan_30.xml").c_str());
			carter28Font = FontRenderer::LoadFromFile((basePath + "fonts/carter_28.xml").c_str());
			carter24Font = FontRenderer::LoadFromFile((basePath + "fonts/carter_24.xml").c_str());
			Utils::LoadGuiMap(basePath + "gui/768/gui.map");
			Utils::LoadGuiProperties(basePath + "gui/768/");
			break;

		case 768:
			defaultFont = FontRenderer::LoadFromFile((basePath + "fonts/komika_title_32.xml").c_str());
			Utils::LoadGuiMap(basePath + "gui/768/gui.map");
			Utils::LoadGuiProperties(basePath + "gui/768/");
			break;

		case 320:
			defaultFont = FontRenderer::LoadFromFile((basePath + "fonts/komika_title_16.xml").c_str());
			Utils::LoadGuiMap(basePath + "gui/320/gui.map");
			Utils::LoadGuiProperties(basePath + "gui/320/");
			break;
	}
	
	assert(defaultFont != NULL);
	ClassContainer::GetInstance()->AddClass("defaultFont", defaultFont);
	ClassContainer::GetInstance()->AddClass("allan18Font", allan18Font);
	ClassContainer::GetInstance()->AddClass("allan30Font", allan30Font);
	ClassContainer::GetInstance()->AddClass("carter28Font", carter28Font);
	ClassContainer::GetInstance()->AddClass("carter24Font", carter24Font);
	
	return true;
}

void Game::Update(float time, float ms)
{
	if (isWaitingForFade && Fade2::GetInstance()->IsReady())
	{
//		if (dynamic_cast<SplashScreen*>(this->gameState) != NULL)
//			delete this->gameState;
		
		isWaitingForFade = false;
		this->gameState = nextGameState;
		nextGameState = NULL;
		
		Fade2::GetInstance()->StartFadeOut(0.4f);
		
		gameState->SetFocus();
	}
	
	gameState->Update(time, ms);
	if (optionsPanel->IsActive())
		optionsPanel->Update(time, ms);
	
	if (msgBoxMan->IsMsgBoxVisible())
		msgBoxMan->GetMessageBox()->Update(time, ms);
	
	Fade2::GetInstance()->Update(time, ms);
}

void Game::Draw(float time, float ms)
{	
	gameState->Draw(time, ms);
	
	spriteBatch->Begin();
	if (optionsPanel->IsActive())
		optionsPanel->Draw(time, ms);
	
	if (msgBoxMan->IsMsgBoxVisible())
		msgBoxMan->GetMessageBox()->Draw(time, ms);
	spriteBatch->End();
	
	Fade2::GetInstance()->Draw(time, ms);
}

void Game::HandlePinchGesture(GestureStatus status, float scale, float velocity)
{
	if (msgBoxMan->IsMsgBoxVisible() || optionsPanel->IsActive())
		return;

	gameState->HandlePinchGesture(status, scale, velocity);
}

void Game::HandlePanGesture(GestureStatus status,
							const sm::Vec2 &pos,
							const sm::Vec2 &trans,
							const sm::Vec2 &velocity)
{
	if (msgBoxMan->IsMsgBoxVisible())
		return;
	
	if (optionsPanel->IsActive())
		optionsPanel->HandlePanGesture(status, pos, trans, velocity);
	else
		gameState->HandlePanGesture(status, pos, trans, velocity);
}

void Game::HandleTapGesture(const sm::Point<int> &point)
{
	if (msgBoxMan->IsMsgBoxVisible())
		msgBoxMan->GetMessageBox()->HandleTapGesture(point);
	else if (optionsPanel->IsActive())
		optionsPanel->HandleTapGesture(point);
	else
		gameState->HandleTapGesture(point);
}

void Game::HandlePress(const sm::Point<int> &point)
{
	if (msgBoxMan->IsMsgBoxVisible())
		msgBoxMan->GetMessageBox()->HandlePress(point);
	else if (optionsPanel->IsActive())
		optionsPanel->HandlePress(point);
	else
		gameState->HandlePress(point);
}

void Game::HandleRelease(const sm::Point<int> &point)
{
	if (msgBoxMan->IsMsgBoxVisible())
		msgBoxMan->GetMessageBox()->HandleRelease(point);
	else if (optionsPanel->IsActive())
		optionsPanel->HandleRelease(point);
	else
		gameState->HandleRelease(point);
}

void Game::SetGameState(IGameState *gameState)
{
	if (nextGameState == gameState)
		return;

	// TODO: wsadz to w SetFocus w gamestate
	if (dynamic_cast<SelectLevelGameState*>(gameState) != NULL)
		selectLevelGameState->ScrollToFirstUnlockedUnresolvedLevel();
	
	isWaitingForFade = true;
	nextGameState = gameState;
	
	Fade2::GetInstance()->StartFadeIn(0.4f);
}

void Game::PlayerAuthenticated(const std::string &playerId)
{
	SetupPlayerEnv(playerId);
	
	mainMenuGameState->SetGameCenterButtons(true);
}

void Game::PlayerDeauthenticated()
{
	SetupPlayerEnv(""); // "" means common user
	mainMenuGameState->SetGameCenterButtons(false);
}

void Game::PointsLoaded(unsigned points)
{
	unsigned localPoints = imagesCollection->GetTotalPoints();
	if (localPoints > points)
	{
		Log::LogT("local points (%u) are greather than server. Updating...", localPoints);
		gc->ReportPoints("pixitall.leaderboard.main", localPoints);
	}
	else
		Log::LogT("No need to update points on server");
}

void Game::SetupPlayerEnv(const std::string &playerId)
{
	std::string playerIdDir = playerId.empty() ? "common_user" : Utils::CalcMD5(playerId);
	Log::LogT("Preparing evironment for player %s (dir = %s)", playerId.c_str(), playerIdDir.c_str());
	
	std::string playerDir = docPath + "/levels_stats/" + playerIdDir + "/";
	
	if (!Utils::CreateDir(playerDir))
		Log::LogE("Couldn't create directory: %s", playerDir.c_str());
	
	imagesCollection->LoadLevelStats(playerDir);
	imagesCollection->UnlockLevels();
	selectLevelGameState->ScrollToFirstUnlockedUnresolvedLevel();
	
	if (gc != NULL)
		gc->SetStoredAchievesFile(playerDir + "gc_stored_achieves.dat");
}

void Game::HandleEnterBackground()
{
	soundMng->StopMusic();
	if (playGameState != NULL)
		playGameState->HandleEnterBackground();
}

void Game::HandleEnterForeground()
{
	soundMng->PlayMusic();
}

void Game::PropertyChanged(const std::string &propName, void *sender)
{
	if (sender == soundMng)
	{
		if (propName == "loadingFinished")
		{
			soundMng->PlayMusic();
		}
	}
}

void Game::SendPlayerData()
{
	int playerId = PlayerData::GetInstance()->m_id;
	std::string playerName = PlayerData::GetInstance()->m_name;
	int totalPoints = imagesCollection->GetTotalPoints();
	int totalLevels = imagesCollection->GetFinishedLevelsCount();

	Leaderboard::GetInstance()->SendPlayerPoints(
		playerId,
		playerName,
		totalPoints,
		totalLevels);
}

void Game::HandleBackButton()
{
	if (optionsPanel->IsActive())
	{
		optionsPanel->Close();
		return;
	}

	if (gameState == mainMenuGameState)
		m_systemUtils->QuitApplication();
	else if (gameState == selectLevelGameState)
		SetGameState(mainMenuGameState);
	else if (gameState == playGameState)
		playGameState->TogglePause();
}

void Game::HandleMenukButton()
{
}
