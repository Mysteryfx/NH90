#ifndef _LOCKON_CONTEXT_H_
#define _LOCKON_CONTEXT_H_

#include "Renderer/VolumeEffect.h"
#include "Renderer/Pass.h"
#include "Renderer/Camera.h"
#include "osg/Matrixf"

namespace render
{
	class ConstantBuffer;
	class StructuredBuffer;
}

namespace enlight
{
	class CloudsRenderer;
}

namespace Graphics {

class RenderParser;
class Camera;

class Context 
{
protected:
	Pass			*_pass;
	Camera			_camera;
	RenderParser	*_parser;
	effectState		*_effects;
	enlight::CloudsRenderer	*cldRenderer;

	osg::Vec3d		cameraWorldPos;
	osg::Vec3f 		cameraPos,
					cameraOrigin;

	osg::Vec3f		sunDir, 
					sunDiffuseColor, 
					sunAmbientColor,
					fogColor;

	osg::Matrixf	_view;
	osg::Matrixf	_proj;
	osg::Matrixf	_viewProj;
	osg::Matrixf	_viewInv;

	bool			bInCockpit, bIRmode;

	double			mModelTime;

public:

	GRAPHICSXP_API Context();
	GRAPHICSXP_API ~Context();

	GRAPHICSXP_API static void init();
	GRAPHICSXP_API static void release();
	GRAPHICSXP_API static Context *getCurrentContext();

	GRAPHICSXP_API void reset();

	GRAPHICSXP_API void setPass(Pass *pass);
	inline Pass *getPass() const {return _pass; }

	GRAPHICSXP_API void setCamera(const Camera &camera, bool updateCBuffer = true);
	inline const Camera *getCamera() const {return &_camera;}
	//sunAng.x - высота солнца над горизонтом, рад; sunAng.y - азимут, рад
	GRAPHICSXP_API void updatePerFrameCBuffer(double modelTime, osg::Vec2f *sunAng=0);

	inline const osg::Matrixf &getViewMatrix() const {return _view;}
	inline const osg::Matrixf &getProjMatrix() const {return _proj;}		
	inline const osg::Matrixf &getViewProjMatrix() const {return _viewProj;}
	inline const osg::Matrixf &getViewInverseMatrix() const {return _viewInv;}

	inline const osg::Vec3d	&getCameraWorldPos() const	{ return cameraWorldPos;}
	inline const osg::Vec3f	&getCameraPos() const		{ return cameraPos;}
	inline const osg::Vec3f &getOrigin() const			{ return cameraOrigin;}

	inline const osg::Vec3f &getSunDir()		  const	{ return sunDir;			}
	inline const osg::Vec3f &getSunDiffuseColor() const { return sunDiffuseColor;	}
	inline const osg::Vec3f &getSunAmbientColor() const { return sunAmbientColor;	}
	inline const osg::Vec3f &getFogColor() const		{ return fogColor;	}
	double	getModelTime() const						{ return mModelTime; }

	GRAPHICSXP_API render::ConstantBuffer	&getPerFrameCBuffer() const;
	GRAPHICSXP_API render::ConstantBuffer	&getPerViewCBuffer() const;
	GRAPHICSXP_API render::ConstantBuffer	&getAmbientCBuffer() const;

	inline RenderParser *getCurrentParser() const {return _parser;}
	GRAPHICSXP_API void setCurrentParser(RenderParser *parser);

	inline bool inCockpit() const {return bInCockpit; }
	GRAPHICSXP_API void setInCockpit(bool cockpit);

	GRAPHICSXP_API void setEffects(effectState *effects);
	inline effectState *getEffects() const {return _effects;}

	// Для указанного массива позиций заданных от текущего origin, вычисляет цвет 
	// и непрозрачность облаков с помощью текущего облачного рендера, и складывает в общий структурный буфер. 
	// Возвращает индекс первого элемента в структурном буффере getCloudsColorSBuffer() для чтения цветов.
	// Возвращает -1 если рендер облаков отсутсвует.
	GRAPHICSXP_API int computeCloudsColor(const osg::Vec3f *arr, unsigned count);
	GRAPHICSXP_API const render::StructuredBuffer &getCloudsColorSBuffer() const;
	inline void setCloudsRenderer(enlight::CloudsRenderer *r) { cldRenderer = r; }

	inline bool isIRmode() const	{ return bIRmode;	}
	inline void setIRmode(bool v)	{ bIRmode = v;		}

};

};

#endif
