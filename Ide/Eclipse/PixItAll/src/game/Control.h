#pragma once

#include <list>
#include <map>
#include <string>
#include <Utils/Math/Vec2.h>
#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/IUpdateable.h>
#include "ObserverSubject.h"
#include "ITouchObserver.h"
#include "IGestureHandler.h"
#include <GraphicsLibrary/TexPart.h>
#include <memory.h>

class SpriteBatch;

class Control :
	public virtual IUpdateable,
	public virtual IDrawable,
	public virtual IGestureHandler,
	public ObserversContainer<ITouchObserver*>
{	
protected:
	int x;
	int y;
	int width;
	int height;
	
	float opacity;
	
	bool visible;
	bool enabled;
	
	Control *parent;
	std::list<Control*> children;
	mutable std::map<std::string, unsigned char*> userData;
	
	TexPart bg;
	static SpriteBatch *spriteBatch;
	
	virtual void OnTouch(int x, int y);
	virtual void OnTouchBegin(int x, int y);
	virtual void OnTouchEnd(int x, int y);
	virtual void OnDraw(float time, float ms);
	virtual void OnUpdate(float time, float ms);
	
public:
	static void SetSpriteBatch(SpriteBatch *spriteBatch);
	
	Control();
	Control(int x, int y, int width, int height);
	Control(int x, int y, const TexPart &bg);
	Control(int x, int y, int width, int height, const TexPart &bg);
	virtual ~Control();
	
	Control *GetParent();
	void AddChild(Control *control);
	void RemoveChild(Control *control);
	
	void SetX(int x);
	void SetY(int y);
	void SetPosition(int x, int y);
	void SetSize(int width, int height);
	void SetBounds(int x, int y, int width, int height);
	void SetVisible(bool visible);
	void SetEnabled(bool enabled);
	
	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;
	bool IsVisible() const;
	bool IsEnabled() const;
	
	float GetOpacity() const;
	void SetOpacity(float opacity);
	
	sm::Vec2 GetLocalPos();
	sm::Vec2 GetGlobalPos();
	
	void HandlePanGesture(IGestureHandler::GestureStatus status,
						  const sm::Vec2 &pos,
						  const sm::Vec2 &trans,
						  const sm::Vec2 &velocity);
	void HandleTapGesture(const sm::Point<int> &point);
	void HandlePress(const sm::Point<int> &point);
	void HandleRelease(const sm::Point<int> &point);

	virtual bool HitTest(int x, int y) const;
	
	virtual void Update(float time, float ms);
	virtual void Draw(float time, float ms);
	
	template <typename T>
	void SetUserData(const std::string &name, const T &value)
	{
		unsigned char *valCopy = new unsigned char[sizeof(T)];
		memcpy(valCopy, &value, sizeof(T));
		userData[name] = valCopy;
	}
	
	template <typename T>
	bool GetUserData(const std::string &name, T &value) const
	{
		std::map<std::string, unsigned char*>::iterator it = userData.find(name);
		if (it == userData.end())
			return false;
		
		memcpy(&value, it ->second, sizeof(T));
		return true;
	}
};
