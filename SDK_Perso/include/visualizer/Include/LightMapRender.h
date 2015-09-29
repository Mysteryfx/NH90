#pragma once
#include "LightRenderData.h"
#include "renderer/IRenderAPI.h"
#include "rwrappers/renderUserAPI.h"


class LightMapRender 
{
public:
	LightMapRender();
	~LightMapRender();
	
	bool init();

	void renderLightMap(ed::vector<LightRenderData>& lights, const osg::Matrixf& view, 
						               const osg::Matrixf& proj, const osg::Vec3f& origin);

	void getTTFMatrix(osg::Matrixf& out_ttf, const osg::Matrixf& view, const osg::Matrixf& projection, const osg::Vec3f& origin);
private:
	void getTTFMatrix(const LightRenderData& light, const osg::Matrixf& world, osg::Matrixf& ttf);
	void render(const LightRenderData& light, const osg::Matrixf& lightViewProj, const osg::Vec3f& origin);
	
	// При инициализации
	render::Shader shader;
	render::handle_t tOmni, tSpot;

	render::handle_t shader_W;
	render::handle_t shader_WVP;
	render::handle_t shader_ttf;
	render::handle_t shader_vAtt;
	render::handle_t shader_vXL;
	render::handle_t shader_color;
	render::handle_t shader_lengthToLand;
	render::handle_t shader_tex;
	render::handle_t pFi, pTheta;
	render::handle_t pOmniPos;

	render::ve_handle_t shader_POSITION0;
	render::ve_handle_t shader_TEXCOORD0;

	class LightPolygonGeometry 
	{
	public:
		LightPolygonGeometry();
		~LightPolygonGeometry();
		
		void init();

		// буфера
		render::VertexBuffer mVertexBuffer;
		render::IndexBuffer mIndexBuffer;
		render::stream_handle_t mStreamP;
		render::PRIMTYPE_ENUM mPrimtype;

		render::Texture mSpotTexture;
		render::Texture mOmniTexture;

		int mFacecount;
	};

	LightPolygonGeometry mGeometry;
};