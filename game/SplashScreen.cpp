#include "SplashScreen.h"
#include <GraphicsLibrary/TexPart.h>
#include <GraphicsLibrary/Texture.h>
#include <GraphicsLibrary/SpriteBatch.h>
#include <Utils/Image.h>
#include <stddef.h>
#include "MainMenuGameState.h"
#include "Game.h"
#include "ClassContainer.h"
#include "Environment.h"

SplashScreen::SplashScreen(Game *game, const std::string &imgPath) :
	BaseGameState(game)
{
	bgTex = NULL;
	this->imgPath = imgPath;
	
	resWidth = Environment::GetInstance()->GetScreenWidth();
	resHeight = Environment::GetInstance()->GetScreenHeight();
}

SplashScreen::~SplashScreen()
{
	Release();
}

Control *SplashScreen::GetMainPanel()
{
	return NULL;
}

void SplashScreen::SetGame(Game *game)
{
	this->game = game;
}

void SplashScreen::Update(float time, float seconds)
{
	if (time > 0.0f)
		game->SetGameState(game->mainMenuGameState);
}

void SplashScreen::Draw(float time, float seconds)
{	
	sb->Begin();
	sb->Draw(bgTex, 0, 0, resWidth, resHeight);
	sb->End();
}

bool SplashScreen::Initialize()
{	
	Image *img = Image::LoadImage(imgPath.c_str());
	if (img == NULL)
	{
		Release();
		return false;
	}
	
	bgTex = new Texture(img->GetWidth(), img->GetHeight(), img->GetBpp(), img->GetData());
	if (bgTex == NULL)
	{
		Release();
		return false;
	}
	
	delete img;
		
	sb = new SpriteBatch();
	if (sb == NULL)
	{
		Release();
		return false;
	}
	
	return true;
}
							
void SplashScreen::Release()
{
	if (bgTex != NULL)
		delete bgTex;
	
	if (sb != NULL)
		delete sb;
}
