//
// Tizen C++ SDK
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
//
// Licensed under the Flora License, Version 1.1 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://floralicense.org/license/
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef _GLES_SHADER_H_
#define _GLES_SHADER_H_


#include <new>
#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FGraphics.h>
#include <FGraphicsOpengl.h>
#include <FGrpFloatMatrix4.h>

class Game;

/**
 * [GlesShader] application must inherit from Application class
 * which provides basic features necessary to define an application.
 */
class GlesShader
	: public Tizen::App::Application
	, public Tizen::Base::Runtime::ITimerEventListener
	, public Tizen::Ui::IKeyEventListener
	, public Tizen::Ui::ITouchEventListener
	, public Tizen::Ui::ITouchTapGestureEventListener
	, public Tizen::Ui::ITouchPanningGestureEventListener
	, public Tizen::Ui::ITouchPinchGestureEventListener
{
public:
	/**
	 * [GlesShader] application must have a factory method that creates an instance of itself.
	 */
	static Tizen::App::Application* CreateInstance(void);

public:
	GlesShader();
	~GlesShader();

	virtual bool OnAppInitializing(Tizen::App::AppRegistry& appRegistry);
	virtual bool OnAppTerminating(Tizen::App::AppRegistry& appRegistry, bool forcedTermination = false);
	virtual void OnForeground(void);
	virtual void OnBackground(void);
	virtual void OnLowMemory(void);
	virtual void OnBatteryLevelChanged(Tizen::System::BatteryLevel batteryLevel);
	virtual void OnScreenOn(void);
	virtual void OnScreenOff(void);

	virtual void OnTimerExpired(Tizen::Base::Runtime::Timer& timer);

	virtual void OnKeyPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode);
	virtual void OnKeyReleased(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode);
	virtual void OnKeyLongPressed(const Tizen::Ui::Control& source, Tizen::Ui::KeyCode keyCode);

	virtual void OnTouchPressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo & touchInfo);
	virtual void OnTouchReleased(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchMoved(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchFocusIn(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchFocusOut(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo);

	virtual void OnTapGestureCanceled (Tizen::Ui::TouchTapGestureDetector &gestureDetector);
	virtual void OnTapGestureDetected (Tizen::Ui::TouchTapGestureDetector &gestureDetector);

	virtual void OnPanningGestureStarted(Tizen::Ui::TouchPanningGestureDetector& gestureDetector);
	virtual void OnPanningGestureChanged(Tizen::Ui::TouchPanningGestureDetector& gestureDetector);
	virtual void OnPanningGestureFinished(Tizen::Ui::TouchPanningGestureDetector& gestureDetector);
	virtual void OnPanningGestureCanceled(Tizen::Ui::TouchPanningGestureDetector& gestureDetector);

	virtual void OnPinchGestureStarted(Tizen::Ui::TouchPinchGestureDetector& gestureDetector);
	virtual void OnPinchGestureChanged(Tizen::Ui::TouchPinchGestureDetector& gestureDetector);
	virtual void OnPinchGestureFinished(Tizen::Ui::TouchPinchGestureDetector& gestureDetector);
	virtual void OnPinchGestureCanceled(Tizen::Ui::TouchPinchGestureDetector& gestureDetector);

	bool Draw(void);
	bool DrawSplashScreen(void);

private:
	bool InitEGL(void);
	bool InitGL(void);
	void DestroyGL(void);
	void Update(void);
	void Cleanup(void);

private:
	Game *m_game;

	Tizen::Ui::Controls::Form*			__pForm;

	Tizen::Graphics::Opengl::EGLDisplay	__eglDisplay;
	Tizen::Graphics::Opengl::EGLSurface	__eglSurface;
	Tizen::Graphics::Opengl::EGLConfig	__eglConfig;
	Tizen::Graphics::Opengl::EGLContext	__eglContext;
	Tizen::Base::Runtime::Timer*			__pTimer;
	Tizen::Graphics::Opengl::GLint		__idxTimeStamp;
	Tizen::Graphics::Opengl::GLfloat		__timeStamp, __strideTimeStamp;
};

#endif
