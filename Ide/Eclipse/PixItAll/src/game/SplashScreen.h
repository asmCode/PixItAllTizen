#pragma once

#include <GraphicsLibrary/TexPart.h>
#include <string>

class Texture;
class SpriteBatch;

class SplashScreen
{
private:
	Texture *bgTex;
	SpriteBatch *sb;
	
	std::string imgPath;
	
	unsigned resWidth;
	unsigned resHeight;
	
public:
	SplashScreen(const std::string &imgPath);
	~SplashScreen();
	
	void Update(float time, float seconds);
	void Draw(float time, float seconds);
	
	bool Initialize();
	void Release();
};
