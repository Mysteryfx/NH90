#ifndef _gEMDraw_h_
#define _gEMDraw_h_

#include "gEM.h"

class gSkyZW;

class gEMDraw : public gEM
{
private:

    gSkyZW* sky;

    void DrawSceneLite(const osg::Vec4f& CamPos,
                       osg::Vec3f& Sun,
                       const osg::Matrixf& m_matView,
                       const osg::Matrixf& m_matProj,
                       bool isOvercast,
					   render::FrameBuffer& cube,
                       double phase, 
					   Graphics::Context *context,
					   int face,
					   float param);

public:
    gEMDraw( const char* name );
    ~gEMDraw();

    void DrawWall(bool isOvercast, render::FrameBuffer& cube, int face, double phase, Graphics::Context *context, float param = 1);
	void DrawTextureToWall(const render::Texture &ref, render::FrameBuffer &cube, int face, Graphics::Context *context);
};

#endif
