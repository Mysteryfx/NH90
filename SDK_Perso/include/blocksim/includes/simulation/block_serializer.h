#ifndef __BLOCK_SERIALIZER_H__
#define __BLOCK_SERIALIZER_H__

#include "lua/lua_loader.h"

#include <assert.h>

class wSimulationBlockDescriptor;
class wBlockParameterContainer;
template <class T> class wBlockParameter;

class wBlockSerializer
{
public:
	virtual Lua::Loader* config() = 0;
	virtual Lua::Loader* params() = 0;	// Should be called only iff runtime == 1

	virtual bool reading() const = 0; 
	virtual bool runtime() const = 0;	// Loading for runtime or edit?

	virtual wSimulationBlockDescriptor* loadBlock() = 0;

	virtual void beginParameters() = 0;
	virtual void serialize(wBlockParameterContainer& cnt) = 0;
	virtual void endParameters() = 0;

	template <class K, class T> void serialize(const K key, T& val)
	{
		if (reading()) config()->get(key, val);
		else config()->set(key, val);
	}
};

//////////////////////////////////////////////////////
// EDITOR VERSION
//////////////////////////////////////////////////////
class wEditorBlockSerializer : public wBlockSerializer
{
public:
	wEditorBlockSerializer(Lua::Loader* ldr, bool reading);

	virtual Lua::Loader* config() { return cfg_; }
	virtual Lua::Loader* params() { assert(0); return 0; }

	virtual bool reading() const { return reading_; }
	virtual bool runtime() const { return false; }

	virtual wSimulationBlockDescriptor* loadBlock();

	virtual void beginParameters();
	virtual void serialize(wBlockParameterContainer& cnt);
	virtual void endParameters() { cfg_->endSubtable(); }

protected:
	Lua::Loader* cfg_;
	bool reading_;
};

//////////////////////////////////////////////////////
// RUNNING VERSION
//////////////////////////////////////////////////////
class wRuntimeBlockSerializer : public wBlockSerializer
{
public:
	wRuntimeBlockSerializer(Lua::Loader* cfgScheme, Lua::Loader* cfgParam);
	virtual ~wRuntimeBlockSerializer();

	virtual bool reading() const { return true; }
	virtual bool runtime() const { return true; }

	virtual Lua::Loader* config() { return cfgScheme_; }
	virtual Lua::Loader* params() { return cfgParam_; }

	virtual wSimulationBlockDescriptor* loadBlock();

	virtual void beginParameters();
	virtual void serialize(wBlockParameterContainer& cnt);
	virtual void endParameters() { cfgScheme_->endSubtable(); }

protected:
	Lua::Loader* cfgScheme_, *cfgParam_;
	ed::string blockName_;
};

#endif 
