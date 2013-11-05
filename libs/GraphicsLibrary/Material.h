#pragma once

#import <string>
#import "../Utils/Math/Vec3.h"
#import "../Utils/IDisposable.h"

class Texture;

class Material : public IDisposable
{
private:

public:
	std::string name;
	
	sm::Vec3 ambientColor;
	sm::Vec3 diffuseColor;
	sm::Vec3 specularColor;
	sm::Vec3 emissiveColor;

	float opacity;
	float glossiness;
	float specularLevel;
	float emissiveAmount;

	std::string diffuseTexName;
	Texture *diffuseTex;

	std::string opacityTexName;
	Texture *opacityTex;

	std::string normalTexName;
	Texture *normalTex;

	Material(void);
	~Material(void);
};
