#pragma once

#import "ITouchObserver.h"
#import <GraphicsLibrary/Color.h>
#include "ObserverSubject.h"

class UserImage;
class IImage;
class IBoardObserver;
class ImageMask;

class BoardManager : public ObserversContainer<IBoardObserver*>
{
private:
	int			boardWidth;
	int			boardHeight;
	UserImage	*userImage;
	IImage		*refImage;
	ImageMask	*imgMask;
	
	static void RevealPixels(UserImage *userImage,
							 IImage *refImage,
							 ImageMask *imgMask);
	
public:
	BoardManager(int width, int height, IImage *refImage, ImageMask *imgMask);
	~BoardManager();
	
	bool Initialize();
	void Deinitialize();
	int GetBorderHeight();
	int GetBorderWidth();
	
	void SetColor(int x, int y, const Color &color);
	
	const IImage* GetUserImage() const;
	const IImage* GetReferenceImage() const;
};
