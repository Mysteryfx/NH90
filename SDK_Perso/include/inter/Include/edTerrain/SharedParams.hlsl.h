#ifndef inter_edTerrain_SharedParams_hlsl_h
#define inter_edTerrain_SharedParams_hlsl_h

namespace render
{
// mirror of SharedParams in RenderContext.hlsl
struct SharedParams
{
	osg::Matrixf prevViewOrigin;
	osg::Matrixf prevProj;

	osg::Vec3f cameraPos;			float dummy000;

	osg::Matrixf cloudShadowsProj;
	osg::Matrixf landHeightmapProj;
	osg::Matrixf lightMapProj;

	osg::Matrixf sunV;								// Vsun: view матрица для солнца
	osg::Matrixf invSunV;							// Vsun: inv view матрица для солнца
	osg::Vec3f sunDirectionV;		float dummy00;	// sunLightDir в V space  

	osg::Vec3f sunDirectionWS;		float sunAttenuation;
	osg::Vec3f moonDirectionWS;		float dummy05;
	osg::Vec4f sunColor;			
	osg::Vec4f moonColor;			
	osg::Vec4f ambientColor;		

	float shadowsPower;
	float civilLights;				osg::Vec2f sdummy123;

	osg::Vec4f fogCoefficients;

	osg::Vec3f fogColor;
	float cameraHeight;	

	osg::Vec3f fogDistances;
	float exposure;	

	osg::Vec4f fogParams;

	osg::Vec3f earthCenter;
	float shadowMapFactor;	

	int heightmapAvailable;			
	int lightmapAvailable;			
	int cascadeShadowsAvailable;
	int atmosphereAvailable;

// shadows
	float		 splitProjDist[4];
	osg::Matrixf shadowMatrix[4];
	osg::Vec3f	 lightDir;
	float		 mapSize;
//
	//shadow map
	osg::Matrixf shadowMapVP;
	osg::Vec2f shadowMapNearFarOrtho; 
	float shadowMapDist;	float sdummy1234;

	inline SharedParams();
	// 
	inline void transpose();

};

}//namespace render

#include "inl/SharedParams.hlsl.inl"

#endif
