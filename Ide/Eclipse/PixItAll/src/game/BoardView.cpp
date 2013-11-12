#import "BoardView.h"
#import "BoardManager.h"
#import "IImage.h"
#import <Content/DemoContent.h>
#import <GraphicsLibrary/Texture.h>
#import <GraphicsLibrary/SpriteBatch.h>
#include <GraphicsLibrary/SpriteAnim.h>
#include <GraphicsLibrary/TexPart.h>
#include <Utils/MemUtils.h>
#include "Pixel.h"
#include "Palette.h"
#include "PalettePanel.h"
#include "ClassContainer.h"
#include "RefImage.h"
#include "UserImage.h"
#include "ParticleManager.h"
#include "Splash.h"
#include "ToolsPanel.h"
#include "Environment.h"
#include "XMLElement.h"
#include <Utils/Log.h>

#include <GraphicsLibrary/OpenglPort.h>

using namespace Tizen::Graphics::Opengl;

const float BoardView::ShowPreviewDuration = 0.15f;
const float BoardView::HidePreviewDuration = 0.25f;
const float BoardView::ScroolSpeedRelax = 12000.0f;

const float BoardView::FirstPreviewDuration = 3.0f;
const float BoardView::GetReadyAfter = 0.7f;
const float BoardView::GetReadyFade = 0.3f;
const float BoardView::GetReadyStill = 0.6f;
const float BoardView::GoAfter = 3.2f;
const float BoardView::GoFade = 0.15f;
const float BoardView::GoStill = 0.5f;

const std::string BoardView::PreviewShownProperty = "PreviewShownProperty";
const std::string BoardView::PreviewHiddenProperty = "PreviewHiddenProperty";
const std::string BoardView::StartedPanProperty = "StartedPanProperty";
const std::string BoardView::EndedPanProperty = "EndedPanProperty";
const std::string BoardView::StartedPinchProperty = "StartedPinchProperty";
const std::string BoardView::EndedPinchProperty = "EndedPinchProperty";

BoardView::BoardView(BoardManager *boardMng,
					 Palette *palette,
					 PalettePanel *palettePanel,
					 bool tutorialMode,
					 ToolsPanel *toolsPanel)
{	
	this ->spriteBatch = NULL;
	
	ClassContainer *cc = ClassContainer::GetInstance();
	XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_BoardView");
	XMLElement *guidefTutorialBoardShift = guidefPanel->GetSubElement(0);

	tutBoardScale = guidefTutorialBoardShift->GetAttributeFloat("BoardScale");
	tutFrameShift.x = guidefTutorialBoardShift->GetAttributeFloat("FrameShiftX");
	tutFrameShift.y = guidefTutorialBoardShift->GetAttributeFloat("FrameShiftY");

	this ->x = 0;
	this ->y = 0;
	this ->width = PixItAll::Environment::GetInstance()->GetScreenWidth();
	this ->height = PixItAll::Environment::GetInstance()->GetScreenHeight() - guidefPanel->GetAttributeInt("heightdiff");
	this ->boardScale = 1.0f;
	this ->lastScale = 1.0f;
	this ->frameShift.Set(0, 0);
	this ->lastFrameShift.Set(0, 0);
	this ->velocity.Set(0, 0);
	this ->needToUpdate = true;
	this->smoothView = false;
	this->refTex = NULL;
	this->previewProgress = 0.0f;
	this->previewMode = false;
	this->previewTime = 0.0f;
	this->tutorialMode = tutorialMode;
	this->previevKeepTryingClose = false;
	this->toolsPanel = toolsPanel;
	
	splash = NULL;
	imgTex = NULL;
	pixelFrameTex = NULL;
	floodTex = NULL;
	floodAnim = NULL;
	
	this->isBonusMode = false;
	
	this ->rowSize = 0;
	this ->columnSize = 0;
	this->isTimeTicking = true;
	
	this ->boardMng = boardMng;
	this ->palette = palette;
	this ->palettePanel = palettePanel;
	
	this ->minScale = 1.0f;
	this ->maxScale = 1.0f;
	
	scaleToUpdate = boardScale;
	shiftToUpdate = frameShift;
	
	memset(partMngQueue, 0, sizeof(ParticleManager*) * ParticleManagerQueueSize);
	partMngQueueIndex = 0;
	splashesProgress = 0.0f;
	getReadyShown = false;
	goShown = false;
	ignoreTap = false;
}

BoardView::~BoardView()
{
	Deinitialize();
}

bool BoardView::Initialize()
{
	if (!LoadContent())
		return false;
	
	spriteBatch = new SpriteBatch();
	if (spriteBatch == NULL)
	{
		Deinitialize();
		return false;
	}
	
	splash = new Splash(spriteBatch);
	
	return true;
}

void BoardView::Deinitialize()
{
	MemUtils::DeleteObject(spriteBatch);
	MemUtils::DeleteObject(splash);
	MemUtils::DeleteObject(refTex);
	MemUtils::DeleteObject(imgTex);
	MemUtils::DeleteObject(floodAnim);
	
	for (unsigned i = 0; i < ParticleManagerQueueSize; i++)
		//MemUtils::DeleteObject(partMngQueue[i]);
		delete partMngQueue[i];
	
	std::list<Pixel*>::iterator it;
	for (it = activePixels.begin(); it != activePixels.end(); it++)
		delete (*it);
	activePixels.clear();
}

bool BoardView::LoadContent()
{
	DemoContent *dc = DemoContent::GetInstance();
	if (dc == NULL)
		return false;
	
	ClassContainer *cc = ClassContainer::GetInstance();
	if (cc == NULL)
		return false;
	
	tpGetReady = cc->GetClass<TexPart>("guimap_get_ready");
	tpGo = cc->GetClass<TexPart>("guimap_go");
		
	pixelFrameTex = dc ->Get<Texture*>("pixel_frame");
	if (pixelFrameTex == NULL)
		return false;
	pixelFrameTex ->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
	bgView = cc->GetClass<TexPart>("guimap_view_bg");
	
	floodTex = dc ->Get<Texture*>("flood_anim");
	if (floodTex == NULL)
		return false;
	
	for (unsigned i = 0; i < ParticleManagerQueueSize; i++)
	{
		partMngQueue[i] = new ParticleManager();
		partMngQueue[i]->Initialize(15, sm::Vec2(0, 0));
	}
	partMngQueueIndex = 0;

	return true;
}

void BoardView::UpdateActivePixels(float time, float ms)
{
	std::list<Pixel*>::iterator it = activePixels.begin();
	while (it != activePixels.end())
	{
		if ((*it) ->IsFinished())
		{
			// after animation set correct color
			imgTex ->SetTextureData((*it)->GetX(), (*it)->GetY(), 1, 1, 32, (void*)(&(*it)->GetColor()));
			
			delete (*it);
			it = activePixels.erase(it);
		}
		else
		{
			(*it) ->Update(time, ms, boardScale, frameShift);
			it++;
		}
	}
}

void BoardView::UpdatePreview(float time, float seconds)
{
	firstPreviewTime += seconds;
	
	if (firstPreviewMode && firstPreviewTime >= FirstPreviewDuration)
	{
		HidePreview();
	}
	
	if (!previewMode && previewProgress == 0.0f)
		return;
	
	if (previevKeepTryingClose)
		HidePreview();
	
	if (previewMode)
	{
		previewTime += seconds;
		
		previewProgress += seconds / ShowPreviewDuration;
		if (previewProgress > 1.0f)
			previewProgress = 1.0f;
	}
	else
	{		
		previewProgress -= seconds / HidePreviewDuration;
		if (previewProgress < 0.0f)
		{
			previewProgress = 0.0f;
			previewMode = false;
			if (firstPreviewMode)
			{
				firstPreviewMode = false;
				toolsPanel->EnablePreviewButton(true);
				isTimeTicking = true;
			}
		}
	}
}

void BoardView::OnUpdate(float time, float ms)
{
	splashesProgress += ms;
	if (!getReadyShown && splashesProgress >= GetReadyAfter && !tutorialMode)
	{
		getReadyShown = true;
		splash->DoSplash(GetReadyFade, GetReadyStill, Splash::SplashType_Opacity, tpGetReady);
	}
	if (!goShown && splashesProgress >= GoAfter && !tutorialMode)
	{
		goShown = true;
		splash->DoSplash(GoFade, GoStill, Splash::SplashType_Scale, tpGo);
	}
	
	if (!splash->IsReady())
		splash->Update(time, ms);
	
	UpdatePreview(time, ms);
	
	if (fabs(velocity.x) < 1.0f)
		velocity.x = 0;
	if (fabs(velocity.y) < 1.0f)
		velocity.y = 0;
	
	if (velocity != sm::Vec2(0.0f, 0.0f))
	{
		float revSpeed = ms * ScroolSpeedRelax;
		if (revSpeed >= velocity.GetLength())
			velocity = sm::Vec2(0, 0);
		else
		{
			sm::Vec2 revVelocity = velocity.GetReversed().GetNormalized() * revSpeed;
			velocity += revVelocity;
		}
	}
	
	shiftToUpdate -= velocity * ms * (boardScale / Pixel::BaseWidth);
	
	if (scaleToUpdate != boardScale || shiftToUpdate != frameShift)
	{
		if (!smoothView)
		{
			SetBoardView(scaleToUpdate, shiftToUpdate);
			scaleToUpdate = boardScale;
			shiftToUpdate = frameShift;
		}
		else
		{			
			//float val = -((smoothStep - 1) * (smoothStep - 1)) + 1;
			float val = 1 - pow(2048.0f, -(smoothStep*smoothStep));
			SetBoardView(smoothBaseScale * (1.0f - val) + scaleToUpdate * val,
						 smoothBaseShift * (1.0f - val) + shiftToUpdate * val);
			
			if (smoothStep == 1.0f)
			{
				smoothView = false;
				SetBoardView(scaleToUpdate, shiftToUpdate);
				scaleToUpdate = boardScale;
				shiftToUpdate = frameShift;
			}
			else
			{
				smoothStep = fminf(smoothStep + ms * 2.0f, 1.0f);
			}
		}
	}
	
	if (isBonusMode)
	{
		for (unsigned i = 0; i < ParticleManagerQueueSize; i++)
		{
			if (partMngQueue[i]->IsActive())
			{
				partMngQueue[i]->Update(time, ms);
				partMngQueue[i]->SetPosition(BoardToScreen(partPositions[i].x, partPositions[i].y));
			}
		}
	}
		
	UpdateActivePixels(time, ms);
}

void BoardView::OnDraw(float time, float ms)
{
	spriteBatch->Draw(bgView, 0, 0);
	
	DrawBoard();
			
	std::list<Pixel*>::iterator it;
	for (it = activePixels.begin(); it != activePixels.end(); it++)
		(*it) ->Draw(time, ms);
	
	if (isBonusMode)
	{
		for (unsigned i = 0; i < ParticleManagerQueueSize; i++)
		{
			if (partMngQueue[i]->IsActive())
				partMngQueue[i]->Draw(time, ms);
		}
	}
	
	if (!splash->IsReady())
		splash->Draw(time, ms);
}

void BoardView::BoardInitialized()
{
	const IImage *userImg = boardMng ->GetUserImage();
	rowSize = userImg ->GetWidth();
	columnSize = userImg ->GetHeight();
	
	// use pixels from user image, since there might be some resolved pixels (from mask)
	imgTex = new Texture(rowSize, columnSize, 32, ((UserImage*)userImg)->GetColors(), false);
	
	imgTex ->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	const RefImage *refImage = (RefImage*)boardMng->GetReferenceImage();
	refTex = new Texture(refImage->GetWidth(), refImage->GetHeight(), 32, refImage->GetColors(), false);
	refTex->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);	
		
	baseWidth = (float)width / (float)Pixel::BaseWidth;
	baseHeight = (float)height / (float)Pixel::BaseHeight;
	
	floodAnim = new SpriteAnim(floodTex, 80, 80, 11, 103);
	floodAnim ->SetAnimationDuration(Pixel::ColorFloodDuration);
	
	maxScale = (float)width / ((float)Pixel::BaseWidth * 1.5f * (float)baseWidth);
	minScale = fmaxf((float)rowSize / (float)baseWidth,
					 (float)columnSize / (float)baseHeight);

	if (tutorialMode)
	{
		firstPreviewMode = false;
		frameShift = tutFrameShift;
		boardScale = tutBoardScale;
		lastScale = 1.0f;
		isTimeTicking = true;
	}
	else
	{
		firstPreviewMode = true;
		firstPreviewTime = 0.0f;
		SetBoardView(minScale, sm::Vec2(0.0f, 0.0f));
		lastScale = 1.0f;
		toolsPanel->EnablePreviewButton(false);
		ShowPreview();
		isTimeTicking = false;
	}
	
	scaleToUpdate = boardScale;
	shiftToUpdate = frameShift;
}

void BoardView::PickedWrongColor(int x, int y, const Color &color)
{
}

void BoardView::PickedCorrectColor(int x, int y, const Color &color)
{
	// correct pixel in imgTex will be set after finished pixel animation
	
	Pixel *pixel = new Pixel(spriteBatch,
							 floodAnim,
							 TexPart(pixelFrameTex),
							 x, y);
	pixel ->StartCorrectColorAnim(palette ->GetActiveColor());
	activePixels.push_back(pixel);
	
	if (isBonusMode)
		StartBonusOnPixel(x, y);
}

void BoardView::StartBonusOnPixel(int x, int y)
{
	partPositions[partMngQueueIndex] = sm::Vec2(x, y);
	partMngQueue[partMngQueueIndex]->SetPosition(BoardToScreen(x, y));
	partMngQueue[partMngQueueIndex]->Blow(0.7f, 220.0f, 0.3f);
	
	partMngQueueIndex++;
	if (partMngQueueIndex == ParticleManagerQueueSize)
		partMngQueueIndex = 0;
}

void BoardView::OnTouch(int x, int y)
{
}

bool BoardView::IsVisible()
{
	return true;
}

bool BoardView::IsClosing()
{
	return false;
}

void BoardView::HandlePinchGesture(GestureStatus status, float scale, float velocity)
{
	scale = 1.0f / scale;
	
	if (status == GestureStatus_Began)
	{
		lastScale = boardScale;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, StartedPinchProperty, this);
	}
	if (status == GestureStatus_Changed)
	{
		smoothView = false;
		scaleToUpdate = scale * lastScale;
	}
	if (status == GestureStatus_Ended)
	{
		ignoreTap = false;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, EndedPinchProperty, this);
	}
	
	needToUpdate = true;
}

void BoardView::HandlePanGesture(GestureStatus status,
								 const sm::Vec2 &pos,
								 const sm::Vec2 &trans,
								 const sm::Vec2 &velocity)
{
	if (status == GestureStatus_Began)
	{
		lastFrameShift = frameShift;
		this ->velocity.Set(0, 0);
		
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, StartedPanProperty, this);
	}	
	if (status == GestureStatus_Changed)
	{				
		smoothView = false;
		shiftToUpdate = (trans.GetReversed() * (boardScale / (float)Pixel::BaseWidth)) + lastFrameShift;
	}
	if (status == GestureStatus_Ended)
	{
		ignoreTap = false;
		this ->velocity = velocity;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, EndedPanProperty, this);
	}
	
	needToUpdate = true;
}

void BoardView::HandleTapGesture(const sm::Point<int> &point)
{
	if (ignoreTap)
	{
		ignoreTap = false;
		return;
	}
	
	if (firstPreviewMode)
		return;
	
	if (!palettePanel->IsRolled())
		return;
	
	sm::Vec2 globalPos = GetGlobalPos();
	
	int x = point.X - globalPos.x;
	int y = point.Y - globalPos.y;
	
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
	
	float frameWidth = (float)Pixel::BaseWidth * (1.0f / boardScale);

	float wtf = (float)ClickableFrameThreshold;

	if (frameWidth < wtf && !smoothView)
	{
		smoothView = true;
		smoothStep = 0.0f;
		
		smoothBaseScale = boardScale;
		smoothBaseShift = frameShift;
		
		scaleToUpdate = (float)Pixel::BaseWidth / wtf;
		shiftToUpdate += (sm::Vec2(width / 2 - x, height / 2 - y).GetReversed() * (boardScale / (float)Pixel::BaseWidth));
		
		SetBoardView(scaleToUpdate, shiftToUpdate);
		scaleToUpdate = boardScale;
		shiftToUpdate = frameShift;
		
		boardScale = smoothBaseScale;
		frameShift = smoothBaseShift;		
		
		//printf("zooming: boardScale = %.4f, scaleToUpdate = %.4f\n", boardScale, scaleToUpdate);
	}
	else 
	{
		float boardX = (((float)x * boardScale) / (float)Pixel::BaseWidth + frameShift.x);
		float boardY = (((float)y * boardScale) / (float)Pixel::BaseHeight + frameShift.y);
		
		if (boardX < 0.0f ||
			boardX >= (float)rowSize ||
			boardY < 0.0f ||
			boardY >= (float)columnSize)
			return;
		
		boardMng ->SetColor((int)boardX,
							(int)boardY,
							palette ->GetActiveColor());	
	}
}

void BoardView::DrawBoard()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	float coordX = frameShift.x / (float)rowSize;
	float coordY = frameShift.y / (float)columnSize;
	float coordWidth = ((float)baseWidth * boardScale) / (float)rowSize;
	float coordHeight = ((float)baseHeight * boardScale) / (float)columnSize;
	
	const float coords[8] = {
		coordX, coordY,
		coordX + coordWidth, coordY,
		coordX, coordY + coordHeight,
		coordX + coordWidth, coordY + coordHeight
	};
	
	const float verts[8] = {
		0, 0,
		width, 0,
		0, height,
		width, height
	};
	
	spriteBatch ->Draw(imgTex,
					   verts,	
					   coords,		
					   NULL);
	
	const float pixelFrameCoords[8] = {
		frameShift.x, frameShift.y,
		frameShift.x + (baseWidth * boardScale), frameShift.y,
		frameShift.x, frameShift.y + (baseHeight * boardScale),
		frameShift.x + (baseWidth * boardScale), frameShift.y + (baseHeight * boardScale)
	};
		
	if (IsPrevievActive())
	{
		const unsigned char colorMask1[16] = {
			255, 255, 255, (unsigned char)(255.0f * previewProgress),
			255, 255, 255, (unsigned char)(255.0f * previewProgress),
			255, 255, 255, (unsigned char)(255.0f * previewProgress),
			255, 255, 255, (unsigned char)(255.0f * previewProgress)
		};
		
		spriteBatch ->Draw(refTex,
						   verts,	
						   coords,		
						   colorMask1);
	}
	
	pixelFrameTex->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	spriteBatch ->Draw(pixelFrameTex,
					   verts,
					   pixelFrameCoords,
					   NULL);
					
	float boardWidth = rowSize * (1.0f / boardScale) * Pixel::BaseWidth;
	float boardHeight = columnSize * (1.0f / boardScale) * Pixel::BaseHeight;
	
	if (boardWidth < width)
	{
		float padding = ((float)width - boardWidth + 1) / 2.0f; // +1 because of better precision
		spriteBatch->Draw(Color::White, 0, 0, padding, height);
		spriteBatch->Draw(Color::White, padding + boardWidth, 0, padding, height);
	}
	
	if (boardHeight < height)
	{
		float padding = ((float)height - boardHeight + 1) / 2.0f; // +1 because of better precision
		spriteBatch->Draw(Color::White, 0, 0, width, padding);
		spriteBatch->Draw(Color::White, 0, padding + boardHeight, width, padding);
	}
}

void BoardView::SetBoardView(float scale, const sm::Vec2 &shift)
{
	frameShift = shift;
	
	// calc actual dimensions
	float curWidth = baseWidth * boardScale;
	float curHeight = baseHeight * boardScale;
	
	boardScale = fmaxf(maxScale, fminf(minScale, scale));
	
	float boardWidth = rowSize * (1.0f / boardScale) * Pixel::BaseWidth;
	float boardHeight = columnSize * (1.0f / boardScale) * Pixel::BaseHeight;
			
	// diference between previous and curent dimension
	float scaledPixelsX = curWidth - (baseWidth * boardScale);
	float scaledPixelsY = curHeight - (baseHeight * boardScale);		
	
	if (boardWidth < width)
		frameShift.x = -((baseWidth * boardScale) - (float)rowSize) / 2.0f;
	else
	{
		frameShift.x += scaledPixelsX / 2;
		if (frameShift.x < 0)
			frameShift.x = 0;
		if (baseWidth * boardScale + frameShift.x > rowSize)
			frameShift.x = rowSize - (baseWidth * boardScale);
	}
	
	if (boardHeight < height)
		frameShift.y = -((baseHeight * boardScale) - (float)columnSize) / 2.0f;
	else
	{
		frameShift.y += scaledPixelsY / 2;
		if (frameShift.y < 0)
			frameShift.y = 0;
		if (baseHeight * boardScale + frameShift.y > columnSize)
			frameShift.y = columnSize - (baseHeight * boardScale);
	}
}

void BoardView::HidePreview()
{
	if (tutorialMode && previewTime < 1.0f)
	{
		previevKeepTryingClose = true;
		return;
	}
	
	previevKeepTryingClose = false;
	
	if (previewMode)
	{
		previewMode = false;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, PreviewHiddenProperty, this);
	}
}

void BoardView::ShowPreview()
{
	if (!previewMode)
	{
		previewTime = 0.0f;
		
		previewMode = true;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, PreviewShownProperty, this);
	}
}

bool BoardView::IsPrevievActive()
{
	return previewProgress != 0.0f;
}

void BoardView::ZoomOutMax()
{
	smoothView = true;
	smoothStep = 0.0f;
	
	smoothBaseScale = boardScale;
	smoothBaseShift = frameShift;

	scaleToUpdate = minScale;
	shiftToUpdate.Set(0, 0);
	
	SetBoardView(scaleToUpdate, shiftToUpdate);
	scaleToUpdate = boardScale;
	shiftToUpdate = frameShift;
	
	boardScale = smoothBaseScale;
	frameShift = smoothBaseShift;
}

sm::Vec2 BoardView::BoardToScreen(int x, int y)
{
	return sm::Vec2(((((float)x - frameShift.x) * (float)Pixel::BaseWidth) / boardScale) + (float)Pixel::BaseWidth / (2.0f * boardScale),
					((((float)y - frameShift.y) * (float)Pixel::BaseHeight) / boardScale) + (float)Pixel::BaseHeight / (2.0f * boardScale));
}

void BoardView::SetBonusPixels(bool bonus)
{
	isBonusMode = bonus;
	
	if (isBonusMode)
	{
		for (unsigned i = 0; i < ParticleManagerQueueSize; i++)
			partMngQueue[i]->Stop();
	}
}

bool BoardView::IsFreePreview()
{
	return firstPreviewMode;
}

bool BoardView::IsTimeTicking()
{
	return isTimeTicking;
}

void BoardView::HandlePress(const sm::Point<int> &point)
{
	if (point.Y > height)
		return;
	
	if (velocity.GetLength() > 10.0f)
	{
		velocity = sm::Vec2(0, 0);
		ignoreTap = true;
	}
}
