#include "TextureLoader.h"

#include "../GraphicsLibrary/Texture.h"
#include "../Utils/Utils/Path.h"
#include "../Utils/Image.h"

void *TextureLoader::Load(const char *path)
{
	if (!Path::IsFileExists(path))
		return NULL;
	
	Path filename(path);

	bool genMipmaps = false;

	if (filename.GetFilename() == "gui_01" ||
		filename.GetFilename() == "gui_02" ||
		filename.GetFilename() == "flood_anim")
		genMipmaps = true;

	Image *img = Image::LoadImage(path);
	if (img == NULL)
		return NULL;
	Texture *texture = new Texture(img ->GetWidth(), img ->GetHeight(), img ->GetBpp(), img ->GetData(), genMipmaps);
	delete img;

	return texture;
}
