#ifndef FMB_TYPES_H
#define FMB_TYPES_H

#include <osg/Vec2i>
#include <osg/Matrixd>
#include <stdint.h>

namespace fmb
{
enum RECORDTYPE
{
	RT_EOF = 0, 
	RT_NONE = 0xffffffff, 
	RT_DIRECTORY = 10, 

	// main
	RT_SETCAMERA = 1,				// fmb::RecSetCamera
	RT_RENDERTEXTURE = 2,			// fmb::RecRenderTexture
	RT_SETFOG = 3,					// fmb::RecSetFog	*
	RT_RENDER = 4,					// fmb::RecRender
	RT_SETPASS = 5,					// fmb::RecSetPass
	RT_RENDERTEXT = 6,				// fmb::RecRenderText
	RT_SERVERINFO = 7,				// fmb::RecServerInfo

	// particles
	RT_TERRAIN = 100,				// fmb::RecRenderTerrain	*
	RT_EDM = 101,					// fmb::RecRenderEdm
	RT_SKY = 102,					// fmb::RecRenderSky
	RT_LIGHT = 103,					// fmb::RecLight
};

struct RecDirectory
{
	static const uint32_t packetType = RT_DIRECTORY;
	static const uint32_t packetVersion = 0;
public:
	char name[32];
};

struct RecServerInfo
{
	static const uint32_t packetType = RT_SERVERINFO;
	static const uint32_t packetVersion = 0;
public:
	uint64_t hwnd;
};

struct RecRenderTexture
{
	static const uint32_t packetType = RT_RENDERTEXTURE;
	static const uint32_t packetVersion = 0;
public:
	osg::Vec2i topleft;
	osg::Vec2i rightbottom;
	char texturename[256];
};
struct RecRenderText
{
	static const uint32_t packetType = RT_RENDERTEXT;
	static const uint32_t packetVersion = 0;
public:
	osg::Vec2i topleft;
	char text[256];
};


struct RecSetCamera
{
	static const uint32_t packetType = RT_SETCAMERA;
	static const uint32_t packetVersion = 0;
public:
	osg::Matrixd pos;
	float fovy, 	/// field-of-view angle in the y direction.
	      aspect,	/// aspect ratio
	      _near,	/// near clipping plane
	      middle,
	      _far;		/// far clipping plane
};

struct RecSetPass
{
	static const uint32_t packetType = RT_SETPASS;
	static const uint32_t packetVersion = 0;
public:
	int pass;
};

struct RecRender
{
	static const uint32_t packetType = RT_RENDER;
	static const uint32_t packetVersion = 0;
public:
	int queue;
};

struct RecSky
{
	static const uint32_t packetType = RT_SKY;
	static const uint32_t packetVersion = 0;
public:
	float time;
};

struct RecEDM
{
	static const uint32_t packetType = RT_EDM;
	static const uint32_t packetVersion = 0;
public:
	char modelName[128];
	osg::Matrixd pos;
	uint32_t nTransforms;
	uint32_t nArguments;

	// osg::Matrixd transformMatrices[nTransforms];
	// model::Parameter parameters[nArguments];
};

struct RecLight
{
	static const uint32_t packetType = RT_LIGHT;
	static const uint32_t packetVersion = 0;
public:
	enum enLightType
	{
		ltAmbient = 0,
		ltDirectional = 1,
		ltOmni = 2,
		ltSpot = 3
	};

	enLightType type;
	// позиция
	osg::Matrixd m;
	// цвет
	osg::Vec3f diffuseColor;
	// Радиус пятна (ltOmni, ltSpot)
	float att0, att1, att2, radius;
	// cone angle (ltSpot)
	float fi, theta;
};


}

#endif