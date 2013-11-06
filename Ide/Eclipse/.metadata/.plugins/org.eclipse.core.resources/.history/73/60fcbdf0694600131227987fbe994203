#import "BoardManager.h"
#import "IImage.h"
#import "UserImage.h"
#import <Utils/MemUtils.h>
#import <stddef.h>
#include "IBoardObserver.h"
#include <assert.h>
#include "ImageMask.h"

BoardManager::BoardManager(int width, int height, IImage *refImage, ImageMask *imgMask)
{
	assert(refImage != NULL);
	
	this->boardWidth = width;
	this->boardHeight = height;
	this->refImage = refImage;
	this->imgMask = imgMask;
	
	this ->refImage = refImage;
	
	userImage = UserImage::Create(boardWidth, boardHeight);
	if (userImage == NULL)
	{
		Deinitialize();
	}
	
	userImage ->Clear(Color(0, 0, 0, 0));
	
	// if we have image mask, then reveal some pixels
	if (imgMask != NULL)
		RevealPixels(userImage, refImage, imgMask);
}

BoardManager::~BoardManager()
{
	Deinitialize();
}

void BoardManager::RevealPixels(UserImage *userImage,
								IImage *refImage,
								ImageMask *imgMask)
{
	for (unsigned y = 0; y < imgMask->GetHeight(); y++)
	{
		for (unsigned x = 0; x < imgMask->GetWidth(); x++)
		{
			if (imgMask->IsVisible(x, y))
				userImage->SetColor(x, y, refImage->GetColor(x, y));
		}
	}
}

bool BoardManager::Initialize()
{		
	ObsCast(IBoardObserver, this) ->NotifyObservers(&IBoardObserver::BoardInitialized);
	
	return true;
}

void BoardManager::Deinitialize()
{
	MemUtils::DeleteObject(userImage);
}

void BoardManager::SetColor(int x, int y, const Color &color)
{
	if (userImage ->IsColorResolved(x, y))
		return;
	
	if (color == refImage ->GetColor(x, y))
	{
		userImage ->SetColor(x, y, color);
		ObsCast(IBoardObserver, this) ->NotifyObservers(&IBoardObserver::PickedCorrectColor, x, y, color);
	}
	else
		ObsCast(IBoardObserver, this) ->NotifyObservers(&IBoardObserver::PickedWrongColor, x, y, color);
}

const IImage* BoardManager::GetUserImage() const
{
	return userImage;
}

const IImage* BoardManager::GetReferenceImage() const
{
	return refImage;
}
