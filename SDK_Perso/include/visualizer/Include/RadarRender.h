#ifndef RADARRENDER_H
#define RADARRENDER_H

#include "SceneContainer.h"
#include "rwrappers/renderUserAPI.h"

class SceneManager_Implement;
class smCamera_Implement;

class RadarRender {
public:
	bool	init(SceneManager_Implement &sceneManager);
	void	parse(smCamera_Implement* radarCamera);
	void	draw();

private:
	class Scene: public SceneContainer {
	public:
				void	init(SceneManager_Implement &sceneManager);
		virtual	void	draw();
	};

	Scene scene;
	render::FrameBuffer fbProj, fbBlur;
	render::Shader		shReproject;
	render::Texture		texReproject;

	render::handle_t	tReproject, tClear, tClearPartial, tBlur;
	render::handle_t	pReproj, pInvReproj, pSrc, pSrcDepth, pDst, pSrcInt, pSrcDims, pDstDims, pCameraHeight, pXBounds, pClearOffset;

	osg::Matrixf	reproj, invReproj;
	osg::Vec3f		cameraPos;
	osg::Vec2f		xBounds;

	float			angle, minAngle, maxAngle;
	bool			scanDir;
	double			time;

#ifndef USE_TERRAIN4	
	edtg::TerrainOptions terrainOptionsRadar;
#endif

};


#endif
