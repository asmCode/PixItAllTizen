#pragma once

#include <GraphicsLibrary/Color.h>
#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/IDrawable.h>
#include <Utils/GenericSingleton.h>

class SpriteBatch;

class Fade2 :
	public GenericSingleton<Fade2>,
	public virtual IDrawable,
	public virtual IUpdateable
{
private:
	SpriteBatch *sb;
	Color fadeColor;
	
	float duration;
	
	float fadeValue;
	
	float fadeDirection;
	
	bool ready;
	
	unsigned resWidth;
	unsigned resHeight;
	
public:
	Fade2();
	~Fade2();
	
	void StartFadeIn(float duration);
	void StartFadeOut(float duration);
	
	void Update(float time, float seconds);
	void Draw(float time, float seconds);
	
	bool IsReady() const;
};
