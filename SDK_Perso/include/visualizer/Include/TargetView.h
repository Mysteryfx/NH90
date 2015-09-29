#ifndef _GRAPHIC_UTILS_H_
#define _GRAPHIC_UTILS_H_

#include "Visualizer.h"
#include "rwrappers/renderUserAPI.h"

namespace Graphics {
	class Context;
}

namespace edge
{

class VISUALIZER_API TargetView //: public edge::StdObject, public edge::IRenderable
{
private:
	bool initialized;
	int visible;
	render::VertexBuffer _vBuf;
	render::IndexBuffer _iBuf;

	struct Target
	{
		ed::string strID;

		int type; // 0 - texture, 1 - texture3D, 2 - textureArray
		int x, y, width, height;
		int index;
		float opacity;
		float zoom;

		render::Shader shader;

		render::handle_t viewProjectionMatrixID;
		render::handle_t targetID;
		render::handle_t opacityID;
		render::handle_t zoominvID;
		render::handle_t indexID; // index in textureArray;
		render::handle_t sunColorValue;
		render::handle_t globalAmbientValue;
		render::handle_t cAmbientMap;
		render::handle_t dims; // size of textureInt;

		render::handle_t tech;
	};

	ed::vector<Target> _targets;
	render::stream_handle_t posStream;
	render::stream_handle_t tcStream;

public:
	TargetView();
	virtual ~TargetView();

	/// IRenderable
	virtual void render(Graphics::Context* ctx, const osg::Matrixd& pos, void* data);
//	virtual const osg::BoundingBox& getBoundingBox() const;

//	void updateParam(edge::obj_param_t* msg);

	/// StdObject
//	virtual void init(IScene* scene, obj_id id);
//	virtual void draw(Graphics::Context* context);

	void loadconfig(const char* configname);

	static ed::string getConfigFilename();
	static void getConfigList(ed::vector<ed::string>& targetViewcfgList);
};

}

#endif
