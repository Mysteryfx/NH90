#ifndef __SIMULATION_SYSTEM_DESCRIPTOR_H__
#define __SIMULATION_SYSTEM_DESCRIPTOR_H__

#include "../blocksim.h"

#include <ed/string.h>
#include <assert.h>

#include "simulation_block_disc.h"
#include "simulation_wire_types.h"

class wSimulationSystem;
class wOdeSolverDescriptor;

///////////////////////////////////////////////////////////////
// SCHEME
///////////////////////////////////////////////////////////////
// TODO: надо разделить на схему для редактирования и схему для выполнения
class BLOCKSIM_API wSimulationSystemScheme
{
protected:
	// Содержит входы и выходы схемы
	// В InputPorts/InputWires содержатся ВЫХОДЫ схемы!!!
	class BLOCKSIM_API InputOutputBlock : public wSimulationBlockDiscDescriptor
	{
	public:
		virtual const ed::string& getClassName() const { static ed::string str; return str; };
		virtual const ed::string& getBlockTypeName() const { static ed::string str; return str; }

		virtual void load(Lua::Loader& ldr);
		virtual void save(Lua::Loader& cnf);

		virtual wSimulationBlockDisc* create(Lua::Loader* runtime_cfg, wSimulationSystem*) { assert(0); return 0; }

		void addLead(bool input, bool port, const ed::string& name, eLeadType type);
		void removeLead(bool input, bool port, const ed::string& name);
		void renameLead(bool input, bool port, const ed::string& old_name, const ed::string& new_name);

		virtual wSimulationBlockDescriptor* clone() const { assert(0); return 0; }

	protected:
		virtual const ed::vector<wLeadDescription>& getInputWires_() const { return inputWires_; }
		virtual const ed::vector<wLeadDescription>& getOutputWires_() const { return outputWires_; }
		virtual const ed::vector<wLeadDescription>& getInputPorts_() const { return inputPorts_; }
		virtual const ed::vector<wLeadDescription>& getOutputPorts_() const { return outputPorts_; }

	protected:
		void saveLeads_(Lua::Loader& cfg, ed::vector<wLeadDescription>& desc);
		void loadLeads_(Lua::Loader& ldr, ed::vector<wLeadDescription>& desc);

	protected:
		ed::vector<wLeadDescription> inputWires_, outputWires_, inputPorts_, outputPorts_;
	};

	class BLOCKSIM_API SpecialCommandsBlock : public wSimulationBlockStatelessDescriptor
	{
	public:
		virtual const ed::string& getClassName() const { static ed::string str; return str; };
		virtual const ed::string& getBlockTypeName() const { static ed::string str; return str; }

		virtual void load(Lua::Loader& ldr) {};
		virtual void save(Lua::Loader& cnf) {};

		virtual wSimulationBlockStateless* create(Lua::Loader* runtime_cfg) { assert(0); return 0; }

		virtual wSimulationBlockDescriptor* clone() const { assert(0); return 0; }

	protected:
		virtual const ed::vector<wLeadDescription>& getInputPorts_() const;
	};

public:
	typedef ed::vector<wSimulationBlockDescriptor*> BlocksVector;

	struct Connection
	{
		const wSimulationBlockDescriptor* outputBlock, *inputBlock;
		ed::string outputName, inputName;
		bool port;

		const wSimulationBlockDescriptor* getBlock(bool input) { return input ? inputBlock : outputBlock; }
		const ed::string& getName(bool input) { return input ? inputName : outputName; }

		bool operator == (const Connection& rhs) const
		{
			return	outputBlock == rhs.outputBlock &&
					inputBlock == rhs.inputBlock &&
					outputName == rhs.outputName &&
					inputName == rhs.inputName &&
					port == rhs.port;
		}

		bool operator != (const Connection& rhs) const
		{
			return !(*this == rhs);
		}
	};

	typedef ed::vector<Connection> ConnectionsVector;
public:
	wSimulationSystemScheme();
	wSimulationSystemScheme(const wSimulationSystemScheme& s);
	~wSimulationSystemScheme();

	wSimulationSystem* instantiate(Lua::Loader* runtime_config, lead_data_ptr_t memory) const;

	void loadForEdit(const ed::string& filename, ed::vector<int>& discarded_connections, bool subsystem);
	void loadForRuntime(const ed::string& filename, Lua::Loader* paramsCfg, bool subsystem);

	void save(const ed::string& filename);

	// clone
	wSimulationSystemScheme* clone() const;

	// Memory
	int getMemorySize() const { return memorySize_; }
	int getDiffMemorySize() const { return diffMemSize_; }
	int getDiffMemoryOffset() const { return diffMemOffset_; }

	// Solver
	const wOdeSolverDescriptor* getOdeSolverDescriptor() const { return odeSolverDesc_; }
	void setOdeSolverDescriptor(wOdeSolverDescriptor* desc);

	// blocks stuff
	int getBlocksCount() const { return (int)blocks_.size(); }
	int getStatelessBlocksCount() const { return statelessCnt_; }
	int getDiscreteBlocksCount() const { return discCnt_; }
	int getDiffBlocksCount() const { return diffCnt_; }
	int getStatelessBlockIndex(int i) const { assert(i >= 0 && i < statelessCnt_); return i; }
	int getDiscreteBlockIndex(int i) const { assert(i >= 0 && i < discCnt_); return discOfs_ + i; }
	int getDiffBlockIndex(int i) const { assert(i >= 0 && i < diffCnt_); return diffOfs_ + i; }
	const ed::vector<int>& getStatelessBlocksOnInput(int i) const { assert(i >= 0 && i < discCnt_); return stBlocksOnInput_[i]; }
	const ed::vector<int>& getStatelessBlocksOnOutput(int i) const { assert(i >= 0 && i < discCnt_); return stBlocksOnOutput_[i]; }
	const ed::vector<int>& getStatelessBlocksOnSchemeInput() const { return stBlocksOnSchemeInput_; }
	const ed::vector<int>& getStatelessBlocksOnSchemeOutput() const { return stBlocksOnSchemeOutput_; }
	const ed::vector<int>& getStatelessBlocksOnDiffInput() const { return stBlocksOnDiffInput_; }
	const ed::vector<int>& getStatelessBlocksInDiff() const { return stBlocksInDiff_; }

	// Для редактирования
	const BlocksVector& getBlocks() const { return blocks_; };
    wSimulationBlockDescriptor* getBlockByName(const ed::string& name);
    wSimulationBlockDescriptor* getBlockByIndex(int index);
    const wSimulationBlockDescriptor* getBlockByName(const ed::string& name) const;
	int indexOfBlock(const wSimulationBlockDescriptor* desc) const;
	void addBlock(wSimulationBlockDescriptor* desc);
	void removeBlock(wSimulationBlockDescriptor* desc);
	void renameBlock(wSimulationBlockDescriptor* desc, const ed::string& name);

	// order
	int  getBlockOrder(wSimulationBlockDescriptor* desc);
	void reorderBlock(wSimulationBlockDescriptor* desc, int new_order);

	const ConnectionsVector& getConnections() const { return connections_; }
	void addConnection(const Connection& con);
	void removeConnection(const Connection& con);
	int indexOfConnection(const Connection& con);

	const wSimulationBlockDescriptor* getIOBlock() const { return &ioBlock_; }
	wSimulationBlockDescriptor* getIOBlock() { return &ioBlock_; }
	const ed::vector<wLeadDescription>& getIOLeads(bool input, bool port) const;
	void addIOLead(bool input, bool port, const ed::string& name, eLeadType type);
	void removeIOLead(bool input, bool port, const ed::string& name);
	void renameIOLead(bool input, bool port, const ed::string& old_name, const ed::string& new_name);

	const wSimulationBlockDescriptor* getSpecialCmdsBlock() const { return &spCmdsBlock_; }
	wSimulationBlockDescriptor* getSpecialCmdsBlock() { return &spCmdsBlock_; }

	const ed::string& getRootDir() { return rootDir_; }

	// debug dump
	void debug_dump();

protected:
	void load_(const ed::string& filename, Lua::Loader* params, ed::vector<int>* discarded_connections, bool subsystem);
	void postLoad_(bool subsystem);
	void sortStateless_();
	ed::vector<ed::vector<int> > buildGraph_(int n);	// построить граф соединений первых n блоков

	int indexOfConnection_(const Connection& con);
	bool findInputConnection_(const wSimulationBlockDescriptor* block, const ed::string& name);

protected:
	BlocksVector blocks_;				// в рантайме отсорчено STATELESS - DIFF - DISC
										// stateless отсорчены топологически
	ConnectionsVector connections_;

	InputOutputBlock ioBlock_;
	SpecialCommandsBlock spCmdsBlock_;

	// LAYOUT
	// Это должен модифицировать systemLayoutBuilder
	int statelessCnt_, diffCnt_, discCnt_;
	int diffOfs_, discOfs_;
	ed::vector<int> stBlocksOnSchemeInput_; 
	ed::vector<int> stBlocksOnSchemeOutput_; 
	ed::vector<ed::vector<int> > stBlocksOnInput_;			// только для DISC
	ed::vector<ed::vector<int> > stBlocksOnOutput_;			// только для DISC
	ed::vector<int> stBlocksOnDiffInput_;	// stateless блоки на входе неприрывного куска
	ed::vector<int> stBlocksInDiff_;		// stateless блоки в неприрывном куске (в том числе на выходе)

	// Размер требуемой памяти
	int memorySize_;
	// Смещение и размер блока с состоянием неприрывного куска
	int diffMemSize_, diffMemOffset_;

	// решатель
	wOdeSolverDescriptor* odeSolverDesc_;

	// Misc stuff
	ed::string rootDir_;

	// Что-то мне это не нравится
	friend class wSimulationSystemLayoutBuilder;
};

#endif
