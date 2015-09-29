#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TRANSPORT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TRANSPORT_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef PARTICLE_EXPORTS
#define PARTICLE_API __declspec(dllexport)
#else
#define PARTICLE_API __declspec(dllimport)
#endif


#ifndef EDGE

namespace osg { class Matrixd; class Vec3f;}
namespace Graphics { class ModelParser;}

struct lua_State;
class Position3;
class LockOnPS;
class viObject;

#include <ed/string.h>
#include "Mem/Shared.h"
#include "EffectHolder.h"

//пока без принудительного убиения эффектов


//Этот дополнительный уровень вложенности нужен, чтобы в проектах использующих LockOnPS 
//не было зависимостей от GraphicEffectImpl
//также LockOnPS удаляется только из своек активности
//EffectHolder позволяет всем ссылкам на этот эффект знать, что он удалён, при этом сами ссылки остаются рабочими
//по факту нужен только для останавливаемых из мира эффектов (пыли от мащин, вертолётов)

class LockOnPS;
namespace PSEffects
{

class PARTICLE_API EffectHolderImpl: public EffectHolder
{
	friend class LockOnPS;
public:
	EffectHolderImpl()
		:_impl(0)
	{}
	virtual ~EffectHolderImpl(){clear();}

	void detach();
	void stop();
	bool isActive();
	void clear();

	void changeLocalPos(const osg::Vec3f &pos);

private:
	friend class PSActivity;
	friend PARTICLE_API Mem::Ptr<EffectHolder> createEffect(const char*, const osg::Matrixd&, float, float, osg::Vec3f*);
	friend PARTICLE_API Mem::Ptr<EffectHolder> createEffect(const char*, const viObject*, const osg::Vec3f&, float, float, osg::Vec3f*);
	LockOnPS* _impl;
};

//инициализация ресурсов
PARTICLE_API void				initialize();
PARTICLE_API void				uninitialize();

//отрисовка
PARTICLE_API void beginRenderPass();
PARTICLE_API void endRenderPass();

//создание эффекта
//duration == -1. - использовать продолжительность указанную в эффекте
PARTICLE_API EffectHolderPtr	createEffect(const char* config, const osg::Matrixd &pos, float scale, float duration = -1.f, osg::Vec3f *color = nullptr);
PARTICLE_API EffectHolderPtr	createEffect(const char* config, const viObject* obj, const osg::Vec3f& offset, float scale, float duration = -1.f, osg::Vec3f *color = nullptr);

PARTICLE_API void preloadEffects();
PARTICLE_API void updateLodFOVcoef(float coef=0);
			float getLodFOVcoef();

//Регистрация консольных комманд для отладки системы
PARTICLE_API ed::string		getCurrentEffect();
PARTICLE_API osg::Vec3f			getCurrentOffset();
PARTICLE_API void				regLua(lua_State * L);
}

#endif
