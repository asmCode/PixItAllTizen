#pragma once

#include <GraphicsLibrary/TexPart.h>
#include "BaseGameState.h"
#include <string>

class Texture;
class SpriteBatch;

class SplashScreen : public BaseGameState
{
private:
	Texture *bgTex;
	SpriteBatch *sb;
	
	std::string imgPath;
	
	unsigned resWidth;
	unsigned resHeight;
	
public:
	SplashScreen(Game *game, const std::string &imgPath);
	~SplashScreen();
	
	Control *GetMainPanel();
	void SetGame(Game *game);
	
	void Update(float time, float seconds);
	void Draw(float time, float seconds);
	
	bool Initialize();
	void Release();
};
