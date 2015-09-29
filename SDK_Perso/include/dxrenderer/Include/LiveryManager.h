#ifndef LIVERY_MANAGER_H
#define LIVERY_MANAGER_H

#include "Resourcer/ILiveryManager.h"
#include "Resourcer/ILivery.h"
#include "Renderer/RendererCallback.h"
#include "ed/unordered_map.h"
#include "rwrappers/renderUserAPI.h"

struct lua_State;

namespace Graphics{

/// Livery/skin manager.
class LiveryManager : public ILiveryManager,public RendererCallback{
public:
	static const unsigned int MAX_TEXTURE_CHANNELS = 16;

	struct Item{
		ed::string material;
		render::Texture texture[MAX_TEXTURE_CHANNELS];
	};	
protected:
	class Livery : public ILivery{
	private:
		const ed::string _name;
	public:
		Livery(const char *name):_name(name), handle(INVALID_LIVERY_HANDLE), value(0){}

		ed::vector<Item>	   items;
		ed::vector<custom_arg> custom_args;
		unsigned int handle;
		/// Only for old cmd models.
		float	     value;

		virtual unsigned int getMaterialHandle(const char *materialName)const;
		virtual unsigned int getNumMaterials()const;
		virtual bool getTexture(unsigned int materialHandle, unsigned int textureChannel, ::render::Texture &tex)const;
		virtual const char* getName()const{return _name.c_str();}
		virtual float getArgumentValue()const{return value;}
		virtual const ed::vector<custom_arg> & getCustomArguments() const { return custom_args; };

		virtual void forceLoad();
	};

	class CompositeLivery : public ILivery {
	private:
		struct Livery{
			ILivery *livery;
			unsigned int handleOffset;
		};		
	public:
		static const unsigned int MAX_LIVERIES = 16;
		Livery liveries[MAX_LIVERIES];
		
		ed::string name;
		ed::vector<custom_arg> custom_args;

		CompositeLivery();
		virtual unsigned int getMaterialHandle(const char *materialName)const;
		virtual unsigned int getNumMaterials()const;
		virtual bool getTexture(unsigned int materialHandle, unsigned int textureChannel, ::render::Texture &tex)const;
		virtual const char* getName()const{return name.c_str();}
		/// As we are not going to support composite liveries for cmd models.
		virtual float getArgumentValue()const{return 0.0f;}
		virtual const ed::vector<custom_arg> & getCustomArguments() const { return custom_args; };

		virtual void forceLoad();
	};

	typedef ed::unordered_map<ed::string,ed::unordered_map<ed::string,unsigned int> > LiveriesByModel;
	/// Stores bindings between liveries and models names.
	/// First map connects model name with list of liveries, second connects livery name with livery handle.
	LiveriesByModel _liveriesByModel;
	
	/// Global list of all liveries.
	ed::vector<ILivery*> _liveries;
	
	/// Lua state for livery files.
	lua_State *L;
public:
	LiveryManager();
	virtual ~LiveryManager();

	// from ILiveryManager
	virtual unsigned int getLiveryHandle(const char *model,const char *livery,bool silent = false);
	virtual unsigned int createCompositeLivery(unsigned int *liveryHandles, unsigned int liveryHandlesSize);
	virtual ILivery* getLivery(unsigned int liveryHandle);

	// from TypedResourceManager
	virtual bool initialize();
	virtual void uninitialize();
	
	// from RendererCallback
	virtual void OnInitialize();
	virtual void OnSetParams();
};

}
#endif
