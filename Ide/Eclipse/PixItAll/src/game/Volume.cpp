#include "Volume.h"
#include <GraphicsLibrary/TexPart.h>
#include "ClassContainer.h"

Volume::Volume()
{
	level = 3;
	
	btnUp = NULL;
	btnDown = NULL;

	for (unsigned i = 0; i < 5; i++)
		imgLevel[i] = NULL;
}

Volume *Volume::Create()
{
	ClassContainer *cc = ClassContainer::GetInstance();
	
	Volume *ret = new Volume();
	if (ret != NULL)
	{	
		ret ->x = 0;
		ret ->y = 0;
		ret ->width = 0; // set layter!
		ret ->height = 0;
		
		TexPart tpVolIcon[5];
		
		TexPart tpPlus = cc->GetClass<TexPart>("guimap_opt_plus");
		TexPart tpPlusDown = cc->GetClass<TexPart>("guimap_opt_plus_down");
		TexPart tpMinus = cc->GetClass<TexPart>("guimap_opt_minus");
		TexPart tpMinusDown = cc->GetClass<TexPart>("guimap_opt_minus_down");
		tpVolIcon[0] = cc->GetClass<TexPart>("guimap_opt_volume_1");
		tpVolIcon[1] = cc->GetClass<TexPart>("guimap_opt_volume_2");
		tpVolIcon[2] = cc->GetClass<TexPart>("guimap_opt_volume_3");
		tpVolIcon[3] = cc->GetClass<TexPart>("guimap_opt_volume_4");
		tpVolIcon[4] = cc->GetClass<TexPart>("guimap_opt_volume_5");
		
		unsigned xShift = 0;

		ret->btnDown = new AnimButton(xShift,
									  (tpVolIcon[4].ImageRect.Height - tpMinus.ImageRect.Height) / 2 + 5,
									  tpMinus,
									  tpMinusDown);
		xShift += tpPlus.ImageRect.Width;
		
		for (unsigned i = 0; i < 5; i++)
		{
			ret->imgLevel[i] = new Control(xShift,
										   0,
										   tpVolIcon[i]);
			
			xShift += ((unsigned)(float)tpVolIcon[i].ImageRect.Width * 0.85f);
			
			ret->AddChild(ret->imgLevel[i]);
		}
		
		xShift += 10;
		ret->btnUp = new AnimButton(xShift,
									(tpVolIcon[4].ImageRect.Height - tpPlus.ImageRect.Height) / 2 + 5,
									tpPlus,
									tpPlusDown);
		xShift += tpMinus.ImageRect.Width;
		
		ret->width = xShift;
		ret->height = tpVolIcon[4].ImageRect.Height;
		
		ret ->AddChild(ret->btnUp);
		ret ->AddChild(ret->btnDown);
		
		ObsCast(ITouchObserver, ret ->btnUp) ->AddObserver(ret);
		ObsCast(ITouchObserver, ret ->btnDown) ->AddObserver(ret);
		
		ret->SetLevel(ret->level);
	}
	
	return ret;
}

void Volume::TouchPressed(Control *control, int x, int y)
{
	if (control == btnUp)
	{
		SetLevel(fminf(level + 1, 5));
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, "u", this);
	}
	else if (control == btnDown)
	{
		SetLevel(fmaxf((int)level - 1, 0));
		ObsCast(IPropertyObserver, this)->NotifyObservers(&IPropertyObserver::PropertyChanged, "d", this);
	}
}

void Volume::SetLevel(unsigned level)
{
	this->level = level;
	
	for (unsigned i = 0; i < 5; i++)
		if ((int)i <= (int)level - 1)
			imgLevel[i]->SetOpacity(0.9f);
		else
			imgLevel[i]->SetOpacity(0.4f);
}

unsigned Volume::GetLevel() const
{
	return level;
}
