#include "PalettePanel.h"
#include "Palette.h"
#include "ColorButton.h"
#include "Environment.h"
#include "ClassContainer.h"
#include "SoundManager.h"
#include "XMLElement.h"
#include <Content/DemoContent.h>

const std::string PalettePanel::PaletteUnrolledProperty = "PaletteUnrolledProperty";
const std::string PalettePanel::PaletteRolledProperty = "PaletteRolledProperty";

PalettePanel::PalettePanel(Palette *palette)
{
	this ->palette = palette;
	visible = true;
	rolled = true;
	isPressed = false;
}

PalettePanel::~PalettePanel()
{
	ColorStripList::iterator it;
	for (it = colorStrips.begin(); it != colorStrips.end(); it++)
		delete (*it);
	colorStrips.clear();
}

PalettePanel *PalettePanel::Create(Palette *palette)
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	PalettePanel *ret = new PalettePanel(palette);
	if (ret != NULL)
	{
		XMLElement *guidefPanel = cc->GetClass<XMLElement*>("guidef_PalettePanel");
		float yShiftStep = guidefPanel->GetAttributeFloat("strip_row_shift");

		ret->resWidth = PixItAll::Environment::GetInstance()->GetScreenWidth();
		ret->resHeight = PixItAll::Environment::GetInstance()->GetScreenHeight();
		
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = ret->resWidth;
		ret ->height = ret->resHeight;
				
		TexPart colorStripTexPart = cc->GetClass<TexPart>("guimap_color_strip");
		
		int yShift = 0.0f;
		const float angleStep = (18.1f / 180.0f) * 3.1415f;
		const float baseAngle = 0.0f;
		float angle = -baseAngle;
		const std::vector<Color> &colors = palette ->GetColors();
		float timeDelay = 0;
		for (unsigned i = 0; i < colors.size(); i++)
		{
			ColorStrip *colorStrip = new ColorStrip(colors[i],
													i,
													sm::Vec2(ret->x + (ret->resWidth / 2),
													ret->y + (ret->resHeight - 60) + yShift),
													colorStripTexPart,
													colorStripTexPart.ImageRect,
													angle -= angleStep,
													timeDelay);

			ret ->colorStrips.push_back(colorStrip);
			ret ->AddChild(colorStrip);
			
			if (i > 0 && (i + 1) % 9 == 0)
			{
				yShift -= yShiftStep;
				angle = -baseAngle;
				timeDelay += 0.15f;
			}
		}
		
		ColorStripList::iterator it;
		int counter = 0;
		for (it = ret->colorStrips.begin(); it != ret->colorStrips.end(); it++)
		{
			if (counter % 9 == 0)
				ret->colorStripRows.push_back(it);
			counter++;
		}
		ret->colorStripRows.push_back(ret->colorStrips.end());
		
		if (!ret->colorStrips.empty())
			(*ret->colorStrips.begin())->SetPeeked(true);
	}
	
	return ret;
}

void PalettePanel::PaletteColorChanged(const Color &color)
{
	std::list<ColorStrip*>::iterator it;
	for (it = colorStrips.begin(); it != colorStrips.end(); it++)
	{
		if ((*it)->GetColor() == color)
			(*it)->SetPeeked(true);
		else
			(*it)->SetPeeked(false);
	}
}

bool PalettePanel::IsVisible()
{
	return visible;
}

bool PalettePanel::IsClosing()
{
	return false;
}

void PalettePanel::Hide()
{
	visible = false;
}

void PalettePanel::Show()
{
	visible = true;
}

void PalettePanel::Unroll()
{
	std::list<ColorStrip*>::iterator it;
	for (it = colorStrips.begin(); it != colorStrips.end(); it++)
		(*it) ->Unroll();
	
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_UnrollPalette);
	
	if (rolled)
	{
		rolled = false;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, PaletteUnrolledProperty, this);
	}
}

void PalettePanel::Roll()
{
	std::list<ColorStrip*>::iterator it;
	for (it = colorStrips.begin(); it != colorStrips.end(); it++)
		(*it) ->Roll();
	
	SoundManager::GetInstance()->PlaySound(SoundManager::Sound_RollPalette);

	if (!rolled)
	{
		rolled = true;
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, PaletteRolledProperty, this);
	}
}

bool PalettePanel::IsRolled()
{
	return rolled;
}

void PalettePanel::Draw(float time, float ms)
{
	OnDraw(time, ms);
		
	ColorStripList::iterator it;
	std::list<ColorStripList::iterator>::reverse_iterator rowsIt;
	for (rowsIt = colorStripRows.rbegin(); rowsIt != colorStripRows.rend(); rowsIt++)
	{
		std::list<ColorStripList::iterator>::reverse_iterator rowsItNext = rowsIt;
		rowsItNext++;
		if (rowsItNext == colorStripRows.rend())
			continue;
		
		for (it = (*rowsItNext); it != (*rowsIt); it++)
			(*it) ->Draw(time, ms);
	}	
}

void PalettePanel::OnTouch(int x, int y)
{
	// do not call Control::OnTouch(), cos we want to handle it manually
	
	if (IsRolled())
		return;
	
	isPressed = false;
	
	ColorStrip *peekedColorStrip = GetColorStripAtPoint(x, y);
	
	if (peekedColorStrip != NULL)
	{
		ClearColorStripStates(true, true);
		
		peekedColorStrip ->SetPeeked(true);
		palette ->SetColor(peekedColorStrip ->GetColorIndex());
	}
	
	Roll();
}

void PalettePanel::HandlePress(const sm::Point<int> &point)
{
	if (IsRolled())
		return;
	
	isPressed = true;
	
	ClearColorStripStates(false, true);

	ColorStrip *peekedColorStrip = GetColorStripAtPoint(point.X, point.Y);
	
	if (peekedColorStrip != NULL)
		peekedColorStrip->SetHoovered(true);
}

void PalettePanel::HandleRelease(const sm::Point<int> &point)
{
//	if (!IsRolled())
//		OnTouch(point.X, point.Y);
}

void PalettePanel::HandlePanGesture(IGestureHandler::GestureStatus status,
									const sm::Vec2 &pos,
									const sm::Vec2 &trans,
									const sm::Vec2 &velocity)
{	
	if (IsRolled())
		return;
	
	isPressed = false;
	
	ColorStrip *peekedColorStrip = GetColorStripAtPoint(pos.x, pos.y);
	
	ClearColorStripStates(false, true);
	
	if (peekedColorStrip != NULL)
	{
		peekedColorStrip ->SetHoovered(true);

		if (status == IGestureHandler::GestureStatus_Ended)
		{
			ClearColorStripStates(true, true);
			
			peekedColorStrip ->SetPeeked(true);
			palette ->SetColor(peekedColorStrip ->GetColorIndex());
			Roll();
		}
	}
}

void PalettePanel::ClearColorStripStates(bool clearPeek, bool clearHoover)
{
	std::list<ColorStrip*>::iterator it;
	for (it = colorStrips.begin(); it != colorStrips.end(); it++)
	{
		if (clearHoover)
			(*it) ->SetHoovered(false);
		
		if (clearPeek)
			(*it) ->SetPeeked(false);
	}
}

ColorStrip *PalettePanel::GetColorStripAtPoint(int x, int y)
{
	ColorStrip *peekedColorStrip = NULL;
	
	float accuracy;
	float minAccuracy = 999.0f;
	
	int minRow = 10;
	int index = 0;

	std::list<ColorStrip*>::iterator it;
	for (it = colorStrips.begin(); it != colorStrips.end(); it++)
	{
		if ((*it) ->HitTest2(x, y, accuracy))
		{
			if (accuracy < minAccuracy && (minRow >= (int)(index / 9)))
			{
				minAccuracy = accuracy;
				peekedColorStrip = (*it);
				minRow = (int)(index / 9);
			}
		}
		
		index++;
	}
	
	return peekedColorStrip;
}

Palette* PalettePanel::GetPalette()
{
	return palette;
}

