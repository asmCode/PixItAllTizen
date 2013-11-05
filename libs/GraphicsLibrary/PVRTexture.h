#import "Texture.h"
#import <vector>
#import <string>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class PVRTexture : public Texture
{
public:
	~PVRTexture();
	PVRTexture(
		unsigned width,
		unsigned height,
		unsigned bpp,
		GLenum internalFormat,
		std::vector<std::string> data
	);
	
	void SetTextureData(const unsigned char *data);
};
