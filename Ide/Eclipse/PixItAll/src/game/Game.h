#pragma once

#include <Utils/Math/Matrix.h>
#include "IGame.h"
#include <string>
#include "IGameCommunityObserver.h"
#include "IScreenKeyboardObserver.h"
#include "IPropertyObserver.h"

class SpriteBatch;
class SplashScreen;
class MainMenuGameState;
class SelectLevelGameState;
class PlayGameState;
class IGameState;
class ImagesCollection;
class OptionsPanel;
class SoundManager;
class MessageBoxManager;
class IGameCommunity;
class IGameCenterViewProxy;

class Game :
	public IGame,
	public IGameCommunityObserver,
	public IPropertyObserver,
	public IScreenKeyboardObserver
{
	friend class SplashScreen;
	friend class MainMenuGameState;
	friend class SelectLevelGameState;
	friend class PlayGameState;
	
private:
	SpriteBatch		*spriteBatch;	
	SplashScreen	*splashScreen;
	MainMenuGameState *mainMenuGameState;
	SelectLevelGameState *selectLevelGameState;
	PlayGameState *playGameState;
	OptionsPanel	*optionsPanel;
	SoundManager	*soundMng;
	IGameCommunity *gc;
	std::string		docPath;
	std::string		basePath;
	
	bool			isWaitingForFade;
	IGameState		*gameState;
	IGameState		*nextGameState;
	ImagesCollection *imagesCollection;
	IGameCenterViewProxy *gameCenterViewProxy;
	
	MessageBoxManager *msgBoxMan;
	
	void SetupPlayerEnv(const std::string &playerId); // setup directories, etc
	
	void PlayerAuthenticated(const std::string &playerId);
	void PlayerDeauthenticated();
	void PointsLoaded(unsigned points);
	
	void PropertyChanged(const std::string &propName, void *sender);
	
	void ScreenKeyboardDone(const std::string& text);
	void ScreenKeyboardCanceled();

public:
	Game(IGameCenterViewProxy *gameCenterViewProxy, SplashScreen *splashScreen);
	~Game();
	
	void SetGameState(IGameState *gameState);
	
	bool Initialize(const std::string &basePath,
					const std::string &docPath);
	void Deinitialize();
	bool LoadContent(const std::string &basePath);
	void Update(float time, float ms);
	void Draw(float time, float ms);
	
	void HandlePinchGesture(IGestureHandler::GestureStatus status, float scale, float velocity);
	void HandlePanGesture(IGestureHandler::GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	void HandleTapGesture(const sm::Point<int> &point);
	void HandlePress(const sm::Point<int> &point);
	void HandleRelease(const sm::Point<int> &point);
	
	void HandleEnterBackground();
	void HandleEnterForeground();

	void SendPlayerData();
};
