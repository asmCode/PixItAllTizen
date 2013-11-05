#include "TextureLoader.h"

#include "../Graphics/Texture.h"
#import "../Utils/Utils/Path.h"
#include "../Utils/Image.h"

void *TextureLoader::Load(const char *path)
{
	if (!Path::IsFileExists(path))
		return NULL;
	
	Image *img = Image::LoadImage(path);
	if (img == NULL)
		return NULL;
	Texture *texture = new Texture(img ->GetWidth(), img ->GetHeight(), img ->GetBpp(), img ->GetData());
	delete img;

	return texture;
}
