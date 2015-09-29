#pragma once

#include <ed/vector.h>
#include "Base.h"
#include "Common.h"
#include "../CallBackSignal.h"
#include "../PtrKeeper.h"
#include <memory>

// #ifdef _CRT_SECURE_NO_WARNINGS
// #pragma message("_CRT_SECURE_NO_WARNINGS def")
// #endif

#define INVERSE(Val) ((Val != 0) ? (1 / (Val)) : 0)

struct lua_State;

namespace EagleFM { namespace Elec {

typedef enum electrical_item_types
{
	e_item_undef = 0,
	e_item_Wire,
	e_item_Switch,
	e_item_Source,
	e_item_SelectorSM,
	e_item_SelectorMM,
	e_item_Adapter,
	e_item_CalcNode,
	e_item_ParallelNode,
	e_item_Consumer,
	e_item_Relay,
	e_item_Battery,
	e_item_ElMach_wShunt,
	e_item_VoltageRegulator,
	e_item_Lamp,
	e_item_Converter,
};

class ItemBase;

namespace HELP_BLOCK
{

bool static IsSourceItem(int id)
{
	return (id == e_item_Source ||
			id == e_item_Battery ||
			id == e_item_ElMach_wShunt ||
			id == e_item_Converter);
};

bool static IsConsumer(int id)
{
	return (id == e_item_Consumer ||
			id == e_item_Lamp ||
			id == e_item_Converter);
};

class ArrayItems : public ed::vector<ItemBase*>
{
	typedef ed::vector<ItemBase*> _base;
public:
	ArrayItems(): m_limitSize(0){};
	virtual ~ArrayItems() {_base::clear();}
	int GetSize() const { return static_cast<int>(_base::size());}
	FMBASE_API ItemBase* Get(size_t i) const;
	FMBASE_API bool Add(ItemBase*);
	void SetLimit(int limit) {m_limitSize = limit;}
	FMBASE_API void Remove(ItemBase*);
private:
	int m_limitSize;
};

}

static const double DC_12_VOLT = 12.0;
static const double DC_26_VOLT = 26.0;
static const double DC_28_VOLT = 28.0;
static const double AC_26_VOLT = 26.0;
static const double AC_36_VOLT = 36.0;
static const double AC_115_VOLT = 115.0;
static const double AC_208_VOLT = 208.0;

class CalcNode;
typedef PtrKeeper<CalcNode> CalcNodePtr;

class FMBASE_API ItemBase : public Common::FakeUnknown<Common::Identifiable>
{
public:
	ItemBase();
	virtual ~ItemBase();

	void clearRefs();

	int getType() const {return Type;}

#ifdef _ED_HIDDEN_DEBUG 
	void setName(const char* name)
	{
		strncpy(m_name, name, 59);
		m_name[59] = 0;
	}

	const char* getName()
	{
		return m_name;
	}
#endif // _ED_HIDDEN_DEBUG 

	void static doTrace3(char *szVal);

	virtual void reset();

	ItemBase* GetOutputItem(int index);
	ItemBase* GetInputItem(int index);

	virtual bool connectBeforeMe(ItemBase* itemIn);
	virtual bool connectAfterMe(ItemBase* itemIn);
	virtual bool connect(ItemBase& itemIn);

	friend ItemBase& operator >> (ItemBase& itemIn1, ItemBase& itemIn2)
	{
		itemIn1.connect(itemIn2);
		return itemIn2;
	}

	virtual void disconnect(ItemBase& itemIn);

	friend void operator - (ItemBase& itemIn1, ItemBase& itemIn2)
	{
		itemIn1.disconnect(itemIn2);
	}

	virtual double getVoltage() const;
	void setVoltage(double valIn) {m_voltage = valIn;}

	virtual bool isPowered() const;

	virtual double getCurrent() const;

	bool IsFailed() const {return m_failed;}
	virtual void setFailure(bool failed);

	void setThresholds(double off, double on);
	double getThresholdOff() const {return m_thresholdOff;}
	double getThresholdOn() const {return m_thresholdOn;}

	virtual double getLoad() const {return m_loadR;}
	virtual void setLoadR(double load);
	virtual void setLoadsR(double load);

	virtual double getConductance() const {return m_conductance;}
	virtual void setConductance(double condIn);
	virtual void updateConductance(double valIn);

	virtual bool canReceiveFrom(const ItemBase* pItem) const;
	virtual bool canTransmitTo(const ItemBase* pItem) const;

	virtual bool checkVoltageChange();

	// apparition / disparition de la tension de cellule
	lwl::CSignal m_signalPowerOn;
	lwl::CSignal m_signalPowerOff;

	lwl::CSignal m_signalFailOn;
	lwl::CSignal m_signalFailOff;

	HELP_BLOCK::ArrayItems& getInputs() {return m_inputs;}
	const HELP_BLOCK::ArrayItems& getInputsConst() const {return m_inputs;}

	HELP_BLOCK::ArrayItems& getOutputs() {return m_outputs;}
	const HELP_BLOCK::ArrayItems& getOutputsConst() const {return m_outputs;}

	virtual bool updateChildrenVoltage();

	virtual void setCalcNodePtr(std::shared_ptr<CalcNodePtr> valIn, ItemBase* inputElement = nullptr);
	CalcNode* getCalcNode();
	std::shared_ptr<CalcNodePtr>& getCalcNodePtr();
	void clearCalcNode();

	double getNominalVoltage() const;
	virtual void setNominalVoltage(double nominal);
	virtual bool UpdateVoltage(double valIn = -1.0);

	virtual void setIsSourceLine() {isSourceLine = true;}
	bool getIsSourceLine() const {return isSourceLine;}

#ifdef _ED_HIDDEN_DEBUG 
	int dbgID;
#endif // _ED_HIDDEN_DEBUG 

protected:
	virtual double convertVoltage(double voltage) {return voltage;}
	virtual void savePreviousVoltage();
	virtual void updateOwnVoltage(double valIn = -1.0);
	virtual double getVoltageFromInputs(double threshold);
	double getVoltageFromInput(ItemBase* input);
	virtual void callOnOffHandlers(int sign);
	void setCalcNode_(const std::shared_ptr<CalcNodePtr>& valIn);

	int Type;

	HELP_BLOCK::ArrayItems m_inputs;
	HELP_BLOCK::ArrayItems m_outputs;

	double m_loadR;	      // élément de résistance de charge
	double m_conductance; // Elément de conductivité résistance de charge correspondant
	double m_thresholdOff, m_thresholdOn;

	bool m_itemIschanging;

	double m_nominalVoltage;
	double m_voltage, mPrevVoltage; // Tension (напряжение)
	bool m_failed; // dommage

	bool isSourceLine;

private:
	std::shared_ptr<CalcNodePtr> calcNode;

#ifdef _ED_HIDDEN_DEBUG 
	char m_name[60];
#endif // _ED_HIDDEN_DEBUG 

};

} }
