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

#include <FIo.h>
#include <math.h>
#include "GlesShader.h"
#include "game/Game.h"
#include <Utils/Log.h>
#include "game/Environment.h"
#include "game/SplashScreen.h"
#include "game/DummyGameCenterViewProxy.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Runtime;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::Graphics::Opengl;


const int TIME_OUT = 0;
const int PAUSE_TIME = 100;

long long lastTicks;
float deltaTime;
float timeSinceStart;

float basePinchScale;

sm::Vec2 lastPanPoint;
sm::Vec2 lastPanTrans;
sm::Vec2 lastPanVelocity;
long long lastPanTicks;

bool firstPanGesture;
bool isPanActive;
bool firstPitchGesture;
bool isPitchActive;

float tapPanThreshold = 30.0f;

class GlesForm
	: public Tizen::Ui::Controls::Form
{
public:
	GlesForm(GlesShader* pApp)
		: __pApp(pApp)
	{
	}

	virtual ~GlesForm(void)
	{
	}

	virtual result OnDraw(void)
	{
		if (__pApp != null)
		{
			__pApp->Draw();
		}
		return E_SUCCESS;
	}

private:
	GlesShader* __pApp;
};

GlesShader::GlesShader(void)
	: __pForm(null)
	, __eglDisplay(EGL_NO_DISPLAY)
	, __eglSurface(EGL_NO_SURFACE)
	, __eglConfig(null)
	, __eglContext(EGL_NO_CONTEXT)
	, __pTimer(null)
	, __idxTimeStamp(0)
	, __timeStamp(1)
	, __strideTimeStamp(0.02)
{
}

GlesShader::~GlesShader(void)
{
}

void
GlesShader::Cleanup(void)
{
	if (__pTimer != null)
	{
		__pTimer->Cancel();
		delete __pTimer;
		__pTimer = null;
	}

	DestroyGL();
}

Application*
GlesShader::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new (std::nothrow) GlesShader();
}

#include <fstream>
#include <Utils/StringUtils.h>

using namespace Tizen::Io;

bool
GlesShader::OnAppInitializing(AppRegistry& appRegistry)
{
	m_game = NULL;

	Log::StartLog(true, false, false);
	result r = E_SUCCESS;
	std::string dataPath;
	std::string writePath;
	TouchTapGestureDetector *touchTapGestureDetector;
	TouchPanningGestureDetector *touchPanGestureDetector;
	TouchPinchGestureDetector *touchPinchGestureDetector;

	Frame* pAppFrame = new (std::nothrow) Frame();
	TryReturn(pAppFrame != null, E_FAILURE, "[GlesShader] Generating a frame failed.");

	r = pAppFrame->Construct();
	TryReturn(!IsFailed(r), E_FAILURE, "[GlesShader] pAppFrame->Construct() failed.");

	this->AddFrame(*pAppFrame);

	__pForm = new (std::nothrow) GlesForm(this);
	TryCatch(__pForm != null, , "[GlesShader] Allocation of GlesForm failed.");

	r = __pForm->Construct(FORM_STYLE_NORMAL);
	TryCatch(!IsFailed(r), delete __pForm, "[GlesShader] __pForm->Construct(FORM_STYLE_NORMAL) failed.");

	//__pForm->SetOrientation(ORIENTATION_LANDSCAPE);

	r = GetAppFrame()->GetFrame()->AddControl(__pForm);
	TryCatch(!IsFailed(r), delete __pForm, "[GlesShader] GetAppFrame()->GetFrame()->AddControl(__pForm) failed.");

	__pForm->SetMultipointTouchEnabled(true);
	__pForm->AddKeyEventListener(*this);
	__pForm->AddTouchEventListener(*this);

	touchTapGestureDetector = new TouchTapGestureDetector();
	touchTapGestureDetector->Construct();
	touchTapGestureDetector->SetTouchCount(1);
	touchTapGestureDetector->SetTapCount(1);
	touchTapGestureDetector->SetMoveAllowance(tapPanThreshold);
	touchTapGestureDetector->AddTapGestureEventListener(*this);
	__pForm->AddGestureDetector(touchTapGestureDetector);

	touchPanGestureDetector = new TouchPanningGestureDetector();
	touchPanGestureDetector->Construct();
	touchPanGestureDetector->SetTouchCount(1);
	touchPanGestureDetector->AddPanningGestureEventListener(*this);
	__pForm->AddGestureDetector(touchPanGestureDetector);

	touchPinchGestureDetector = new TouchPinchGestureDetector();
	touchPinchGestureDetector->Construct();
	touchPinchGestureDetector->AddPinchGestureEventListener(*this);
	__pForm->AddGestureDetector(touchPinchGestureDetector);

	TryCatch(InitEGL(), , "[GlesShader] GlesCube::InitEGL() failed.");

	TryCatch(InitGL(), , "[GlesShader] GlesCube::InitGL() failed.");

	Update();

	DrawSplashScreen();

	__pTimer = new (std::nothrow) Timer;
	TryCatch(__pTimer != null, , "[GlesShader] Failed to allocate memory.");

	r = __pTimer->Construct(*this);
	TryCatch(!IsFailed(r), , "[GlesShader] __pTimer->Construct(*this) failed.");

	dataPath = StringUtils::ToNarrow((App::GetInstance()->GetAppResourcePath()).GetPointer()) + "data/";
	writePath = StringUtils::ToNarrow((App::GetInstance()->GetAppDataPath()).GetPointer());

	SystemTime::GetTicks(lastTicks);

	return true;

CATCH:
	AppLog("[GlesShader] GlesShader::OnAppInitializing eglError : %#x\n"
			"[GlesShader] GlesShader::OnAppInitializing glError : %#x\n"
			"[GlesShader] GlesShader::OnAppInitializing VENDOR : %s\n"
			"[GlesShader] GlesShader::OnAppInitializing GL_RENDERER : %s\n"
			"[GlesShader] GlesShader::OnAppInitializing GL_VERSION : %s\n ",
			static_cast<unsigned int>(eglGetError()),
			static_cast<unsigned int>(glGetError()),
			glGetString(GL_VENDOR),
			glGetString(GL_RENDERER),
			glGetString(GL_VERSION));

	Cleanup();

	return false;
}

bool
GlesShader::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	Cleanup();

	return true;
}

void
GlesShader::OnForeground(void)
{
	if (__pTimer != null)
	{
		__pTimer->Start(TIME_OUT);
	}

	if (m_game != NULL)
		m_game->HandleEnterForeground();
}

void
GlesShader::OnBackground(void)
{
	if (__pTimer != null)
	{
		__pTimer->Cancel();
	}

	if (m_game != NULL)
		m_game->HandleEnterBackground();
}

void
GlesShader::OnLowMemory(void)
{
}

void
GlesShader::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
}

void
GlesShader::OnScreenOn(void)
{
}

void
GlesShader::OnScreenOff(void)
{
}

void
GlesShader::OnTimerExpired(Timer& timer)
{
	if (__pTimer == null)
	{
		return;
	}
	__pTimer->Start(TIME_OUT);

	long long ticks;
	SystemTime::GetTicks(ticks);

	long long deltaTicks = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime = (float)deltaTicks / 1000.0f;

	timeSinceStart += deltaTime;

	if (m_game != NULL)
		m_game->Update(timeSinceStart, deltaTime);

	Update();

	if (!Draw())
	{
		AppLog("[MeshLoader] MeshLoader::Draw() failed.");
	}
}

void
GlesShader::OnKeyPressed(const Control& source, Tizen::Ui::KeyCode keyCode)
{
}

void
GlesShader::OnKeyReleased(const Control& source, Tizen::Ui::KeyCode keyCode)
{
	if (keyCode == Tizen::Ui::KEY_BACK || keyCode == Tizen::Ui::KEY_ESC)
	{
		Terminate();
	}
}

void
GlesShader::OnKeyLongPressed(const Control& source, Tizen::Ui::KeyCode keyCode)
{
}

void GlesShader::OnTouchPressed(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo & touchInfo)
{
	if (m_game != NULL)
		m_game->HandlePress(sm::Point<int>(currentPosition.x, currentPosition.y));
}

void GlesShader::OnTouchReleased(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo)
{
	if (m_game != NULL)
		m_game->HandleRelease(sm::Point<int>(currentPosition.x, currentPosition.y));
}

void GlesShader::OnTouchMoved(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo)
{
	//if (m_game != NULL)
		//m_game->HandleTapGesture(sm::Point<int>(currentPosition.x, currentPosition.y));
}

void GlesShader::OnTouchFocusIn(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo)
{
}

void GlesShader::OnTouchFocusOut(const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo)
{
}

void GlesShader::OnTapGestureCanceled (Tizen::Ui::TouchTapGestureDetector &gestureDetector)
{
}

void GlesShader::OnTapGestureDetected (Tizen::Ui::TouchTapGestureDetector &gestureDetector)
{
	Touch touch;
	Point position = touch.GetPosition(*__pForm);

	if (m_game != NULL)
		m_game->HandleTapGesture(sm::Point<int>(position.x, position.y));
}

void GlesShader::OnPanningGestureStarted(Tizen::Ui::TouchPanningGestureDetector& gestureDetector)
{
	firstPanGesture = true;
	isPanActive = false;

	IList* touchList = gestureDetector.GetTouchInfoListN();

	if (touchList->GetCount() != 1)
		return;

	lastPanVelocity.Set(0, 0);

	TouchInfo* touchInfo = dynamic_cast<TouchInfo*>(touchList->GetAt(0));

	lastPanPoint.x = touchInfo->position.x;
	lastPanPoint.y = touchInfo->position.y;
	SystemTime::GetTicks(lastPanTicks);
}

void GlesShader::OnPanningGestureChanged(Tizen::Ui::TouchPanningGestureDetector& gestureDetector)
{
	if (m_game == NULL)
		return;

	IList* touchList = gestureDetector.GetTouchInfoListN();

	if (touchList->GetCount() != 1)
		return;

	TouchInfo* touchInfo = dynamic_cast<TouchInfo*>(touchList->GetAt(0));

	sm::Vec2 currentPanPoint(touchInfo->position.x, touchInfo->position.y);

	if (firstPanGesture)
	{
		if ((lastPanPoint - currentPanPoint).GetLength() <= tapPanThreshold)
			return;

		lastPanPoint = currentPanPoint;

		firstPanGesture = false;
		isPanActive = true;

		m_game->HandlePanGesture(
			IGestureHandler::GestureStatus_Began,
			lastPanPoint,
			sm::Vec2(0, 0),
			sm::Vec2(0, 0));

			return;
	}

	long long currentPanTicks;
	SystemTime::GetTicks(currentPanTicks);

	lastPanTrans = currentPanPoint - lastPanPoint;
	float deltaTime = (float)(currentPanTicks - lastPanTicks) / 1000.0f;
	lastPanVelocity = lastPanTrans * (1.0f / deltaTime);

	m_game->HandlePanGesture(
			IGestureHandler::GestureStatus_Changed,
			currentPanPoint,
			lastPanTrans,
			lastPanVelocity);
}

void GlesShader::OnPanningGestureFinished(Tizen::Ui::TouchPanningGestureDetector& gestureDetector)
{
	if (m_game == NULL)
		return;

	if (isPanActive)
	{
		isPanActive = false;

		m_game->HandlePanGesture(
				IGestureHandler::GestureStatus_Ended,
				lastPanPoint,
				lastPanTrans,
				lastPanVelocity);
	}
}

void GlesShader::OnPanningGestureCanceled(Tizen::Ui::TouchPanningGestureDetector& gestureDetector)
{

}

void GlesShader::OnPinchGestureStarted(Tizen::Ui::TouchPinchGestureDetector& gestureDetector)
{
	firstPitchGesture = true;
	isPitchActive = false;

	basePinchScale = gestureDetector.GetScaleF();
}

void GlesShader::OnPinchGestureChanged(Tizen::Ui::TouchPinchGestureDetector& gestureDetector)
{
	if (m_game == NULL)
		return;

	if (firstPitchGesture)
	{
		firstPitchGesture = false;
		isPitchActive = true;

		m_game->HandlePinchGesture(
				IGestureHandler::GestureStatus_Began,
				0.0f,
				0.0f);

		return;
	}

	m_game->HandlePinchGesture(
		IGestureHandler::GestureStatus_Changed,
		gestureDetector.GetScaleF() / basePinchScale,
		0.0f);

	Log::LogT("Scale = %.2f", gestureDetector.GetScaleF() / basePinchScale);
}

void GlesShader::OnPinchGestureFinished(Tizen::Ui::TouchPinchGestureDetector& gestureDetector)
{
	if (m_game == NULL)
		return;

	if (isPitchActive)
	{
		isPitchActive = false;
		m_game->HandlePinchGesture(
				IGestureHandler::GestureStatus_Ended,
				0.0f,
				0.0f);
	}
}

void GlesShader::OnPinchGestureCanceled(Tizen::Ui::TouchPinchGestureDetector& gestureDetector)
{
}

void
GlesShader::DestroyGL(void)
{
	if (__eglDisplay)
	{
		eglMakeCurrent(__eglDisplay, null, null, null);

		if (__eglContext)
		{
			eglDestroyContext(__eglDisplay, __eglContext);
			__eglContext = EGL_NO_CONTEXT;
		}

		if (__eglSurface)
		{
			eglDestroySurface(__eglDisplay, __eglSurface);
			__eglSurface = EGL_NO_SURFACE;
		}

		eglTerminate(__eglDisplay);
		__eglDisplay = EGL_NO_DISPLAY;
	}

	return;
}

bool
GlesShader::InitEGL(void)
{
	EGLint numConfigs = 1;

	EGLint eglConfigList[] =
	{
		EGL_RED_SIZE,	8,
		EGL_GREEN_SIZE,	8,
		EGL_BLUE_SIZE,	8,
		EGL_ALPHA_SIZE,	0,
		EGL_DEPTH_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
		EGL_NONE
	};

	EGLint eglContextList[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};

	eglBindAPI(EGL_OPENGL_ES_API);

	__eglDisplay = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
	TryCatch(__eglDisplay != EGL_NO_DISPLAY, , "[GlesShader] eglGetDisplay() failed.");

	TryCatch(!(eglInitialize(__eglDisplay, null, null) == EGL_FALSE || eglGetError() != EGL_SUCCESS), , "[GlesShader] eglInitialize() failed.");

	TryCatch(!(eglChooseConfig(__eglDisplay, eglConfigList, &__eglConfig, 1, &numConfigs) == EGL_FALSE ||
			eglGetError() != EGL_SUCCESS), , "[GlesShader] eglChooseConfig() failed.");

	TryCatch(numConfigs, , "[GlesShader] eglChooseConfig() failed. because of matching config doesn't exist");

	__eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, (EGLNativeWindowType)__pForm, null);
	TryCatch(!(__eglSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS), , "[GlesShader] eglCreateWindowSurface() failed.");

	__eglContext = eglCreateContext(__eglDisplay, __eglConfig, EGL_NO_CONTEXT, eglContextList);
	TryCatch(!(__eglContext == EGL_NO_CONTEXT || eglGetError() != EGL_SUCCESS), , "[GlesShader] eglCreateContext() failed.");

	TryCatch(!(eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) == EGL_FALSE ||
			eglGetError() != EGL_SUCCESS), , "[GlesShader] eglMakeCurrent() failed.");

	return true;

CATCH:
	{
		AppLog("[GlesShader] GlesShader can run on systems which supports OpenGL ES(R) 2.0.");
		AppLog("[GlesShader] When GlesShader does not correctly execute, there are a few reasons.");
		AppLog("[GlesShader]    1. The current device(real-target or emulator) does not support OpenGL ES(R) 2.0.\n"
				" Check the Release Notes.");
		AppLog("[GlesShader]    2. The system running on emulator cannot support OpenGL(R) 2.1 or later.\n"
				" Try with other system.");
		AppLog("[GlesShader]    3. The system running on emulator does not maintain the latest graphics driver.\n"
				" Update the graphics driver.");
	}

	DestroyGL();

	return false;
}

bool
GlesShader::InitGL(void)
{
	glDepthMask(GL_FALSE);

	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

void
GlesShader::Update(void)
{
	int x, y, width, height;
	__pForm->GetBounds(x, y, width, height);

	PixItAll::Environment::GetInstance()->SetScreenSize(width, height);
	glViewport(0, 0, width, height);
}

bool GlesShader::DrawSplashScreen(void)
{
	Log::LogT("Splash Screen");

	if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) == EGL_FALSE ||
			eglGetError() != EGL_SUCCESS)
	{
		AppLog("[GlesShader] eglMakeCurrent() failed.");

		return false;
	}

	int x, y, width, height;
	__pForm->GetBounds(x, y, width, height);

	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::string dataPath = StringUtils::ToNarrow((App::GetInstance()->GetAppResourcePath()).GetPointer()) + "data/splash.png";

	SplashScreen *ss = new SplashScreen(dataPath);
	ss->Initialize();
	ss->Draw(0, 0);
	eglSwapBuffers(__eglDisplay, __eglSurface);

	return true;
}

bool
GlesShader::Draw(void)
{
	if (m_game == NULL)
	{
		std::string dataPath = StringUtils::ToNarrow((App::GetInstance()->GetAppResourcePath()).GetPointer()) + "data/";
		std::string writePath = StringUtils::ToNarrow((App::GetInstance()->GetAppDataPath()).GetPointer());

		IGameCenterViewProxy *viewProxy = new DummyGameCenterViewProxy();
		m_game = new Game(viewProxy, NULL);
		m_game->Initialize(dataPath, writePath);
	}

	if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) == EGL_FALSE ||
			eglGetError() != EGL_SUCCESS)
	{
		AppLog("[GlesShader] eglMakeCurrent() failed.");

		return false;
	}

	int x, y, width, height;
	__pForm->GetBounds(x, y, width, height);

	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_game->Draw(timeSinceStart, deltaTime);

	eglSwapBuffers(__eglDisplay, __eglSurface);

	return true;
}
