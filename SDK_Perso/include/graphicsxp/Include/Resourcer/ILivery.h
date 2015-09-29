#ifndef ILIVERY_H
#define ILIVERY_H

#include "ed/vector.h"

namespace render{
	class Texture;
}

struct custom_arg
{
	unsigned int  arg;
	float	   value;
};
/// Interface for livery objects.
class ILivery{
public:
	virtual ~ILivery(){}

	static const unsigned int NO_MATERIAL = (unsigned int)-1;
	static const unsigned int INVALID_MATERIAL_HANDLE = (unsigned int)-2;

	/// Returns material handle for material with given name.
	/// Returns NO_MATERIAL if livery doesn't have mentions about material with given name.
	/// And this method never returns INVALID_MATERIAL_HANDLE.
	virtual unsigned int getMaterialHandle(const char *materialName)const = 0;

	/// Returns the number of materials contained by livery.
	virtual unsigned int getNumMaterials()const = 0;

	/// Returns texture corresponding to given material handle and given texture channel,
	/// on wrong combination of \param materialHandle and \param textureChannel returns false.
	/// If method failed \param tex will reamin unchanged.
	virtual bool getTexture(unsigned int materialHandle, unsigned int textureChannel, ::render::Texture &tex)const = 0;

	/// Returns name of livery.
	virtual const char* getName()const = 0;

	/// Hack for old cmd models which hold skins inside.
	virtual float getArgumentValue()const = 0;

	virtual const ed::vector<custom_arg> & getCustomArguments() const = 0;

	virtual void forceLoad() = 0;
};
#endif
