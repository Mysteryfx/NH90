#ifndef _gEM_h_
#define _gEM_h_

#include "Resourcer\Plugin.h"

namespace Graphics
{
	class Context;
}

namespace render
{
	class Texture;
	class FrameBuffer;
}

class gEM: public Plugin
{

public:
    gEM( const char *name ) : Plugin( name ) {}
    virtual ~gEM() {}
//self
	virtual void DrawWall(bool isOvercast,render::FrameBuffer& cube, int face, double phase, Graphics::Context *context, float param) = 0;
	virtual void DrawTextureToWall(const render::Texture &ref, render::FrameBuffer &cube, int face, Graphics::Context *context) = 0;
};

#endif
