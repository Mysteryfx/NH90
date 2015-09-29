#ifndef __AMMUNITION_DESCRIPTOR_H__
#define __AMMUNITION_DESCRIPTOR_H__

#include <ed/string.h>

#include "WeaponsBase.h"
#include "wstype.h"
#include "registry/registered.h"

#include "descriptor_base.h"
#include "Ammunition/ammunition_name.h"
#include "simulation_system_scheme.h"

class wDatabaseLoader;

class wAmmunition;
class wAmmunitionDescriptor;
class MovingObject;

namespace Mail { class Stream; }

// Временно - до избавления от wsTypes
WEAPONSBASE_API wAmmunitionDescriptor* wGetAmmunitionDescriptorByType(wsType t);
WEAPONSBASE_API wAmmunitionDescriptor* wGetAmmunitionDescriptorByName(const ed::string & name);

WEAPONSBASE_API wAmmunition* wCreateAmmunitionByDesc(MovingObject * launcher,ObjectID id, wAmmunitionDescriptor * desc, bool server, Lua::Loader* runtime_config);
WEAPONSBASE_API wAmmunition* wCreateAmmunitionByType(MovingObject * launcher,ObjectID id, wsType t, bool server, Lua::Loader* runtime_config);

// Шаблонные
template <class T> const T* wGetAmmunitionDescriptorByType(wsType t)
{
	wAmmunitionDescriptor* d = wGetAmmunitionDescriptorByType(t);
	if (!d || !dynamic_cast<const T*>(d)) return 0;

	return static_cast<const T*>(d);
}

struct wNetCreateInfo;

const ed::string c_drawArgWireName = "draw_arg_";

class WEAPONSBASE_API wAmmunitionDescriptor : public wDescriptorBase, public wNamedAmmunitionDescriptor
{
public:
	wAmmunitionDescriptor(const ed::string& name) : wDescriptorBase(name) {};
	virtual ~wAmmunitionDescriptor() {};

	virtual void load(wDatabaseLoader* loader);

	const wsType& getType() const { return type_; }
	//int getModelTable() const { return model_table_; }
	const ed::string & getModel() const { return model_; }

	const wSimulationSystemScheme* getSystemScheme(bool server) const { return server ? &serverScheme_ : &clientScheme_; }

	// Создать и инициализировать класс боеприпаса
	virtual wAmmunition* createServer(MovingObject * launcher,Lua::Loader* runtime_config) const;
	virtual wAmmunition* createClient(MovingObject * launcher,ObjectID id, Lua::Loader* runtime_config/*, const wNetCreateInfo& info, Mail::Stream& stream*/) const;

	const ed::vector<int>& getDrawArgs(bool server) const { return server ? serverDrawArgs_ : clientDrawArgs_; }

	double getMass() const { return mass_; }

	inline const ed::string & getSounderName()   const { return sounderName_; }
protected:
	virtual wAmmunition* create_() const;

private:
	void initDrawArgs_(const wSimulationSystemScheme* scheme, ed::vector<int>& args);

private:
	// Это временно, потом, видимо, стоит отказаться от wsType
	wsType type_;							// Тип вооружения
	ed::string model_;
	//int model_table_;

	wSimulationSystemScheme serverScheme_;		// Нутро боеприпаса (серверная часть)
	wSimulationSystemScheme clientScheme_;		// Нутро боеприпаса (клиентская часть)

	ed::vector<int> serverDrawArgs_, clientDrawArgs_;	// какие аргументы рисования используются

	ed::string sounderName_;
	double mass_;
};

#endif
