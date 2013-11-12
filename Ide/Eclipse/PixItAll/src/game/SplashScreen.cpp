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

SplashScreen::SplashScreen(const std::string &imgPath)
{
	bgTex = NULL;
	this->imgPath = imgPath;
	
	resWidth = PixItAll::Environment::GetInstance()->GetScreenWidth();
	resHeight = PixItAll::Environment::GetInstance()->GetScreenHeight();
}

SplashScreen::~SplashScreen()
{
	Release();
}

void SplashScreen::Update(float time, float seconds)
{
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
	
	bgTex = new Texture(img->GetWidth(), img->GetHeight(), img->GetBpp(), img->GetData(), true);
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
