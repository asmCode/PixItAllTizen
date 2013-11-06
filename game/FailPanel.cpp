#include "FailPanel.h"
#include <GraphicsLibrary/SpriteBatch.h>
#include <math.h>

FailPanel::FailPanel() :
	activeIconColor(Color::White),
	inactiveIconColor(Color(255, 255, 255, 100))
{
	failCount = 0;
}

FailPanel *FailPanel::Create(SpriteBatch *sb, unsigned failLimit)
{
	ClassContainer *cc = ClassContainer::GetInstance();
		
	FailPanel *ret = new FailPanel();
	if (ret == NULL)
		return NULL;
		
	ret->failIcon = cc->GetClass<TexPart>("guimap_fail_icon");
	ret->failLimit = failLimit;
	ret->sb = sb;
	
	return ret;
}

void FailPanel::ResetFails()
{
	failCount = 0;
}

void FailPanel::MakeFail()
{
	failCount++;
	if (failCount > failLimit)
		failCount = failLimit;
}

void FailPanel::Update(float time, float ms)
{	
	
}

void FailPanel::Draw(float time, float ms)
{
	int xShift = 0;
	
	Color color = activeIconColor;
	
	for (int i = 0; i < failLimit; i++)
	{
		if (i == failCount)
			color = inactiveIconColor;
		
		sb->Draw(failIcon, color, xShift + 2, 2);
		xShift += failIcon.ImageRect.Width + 2;
	}
}
