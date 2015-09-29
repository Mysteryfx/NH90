#ifndef __SIMULATION_SYSTEM_LAYOUT_H__
#define __SIMULATION_SYSTEM_LAYOUT_H__

class wSimulationSystemScheme;
class wSimulationBlockDescriptor;

#include <ed/vector.h>

// Строит информацию о схеме
// Возможно: инлайнит виртуальные подсхемы
// Строит карту памяти
class wSimulationSystemLayoutBuilder
{
protected:
	struct block_;

public:
	wSimulationSystemLayoutBuilder(wSimulationSystemScheme* scheme);
	~wSimulationSystemLayoutBuilder();

	void build();

protected:
	void init_();
	void verifyConnections_();
	void buildMemoryMap_();
	void buildOutputsMap_(int& current_offset);
	void addBlockOutputs_(block_& b, int& current_offset);

	void buildInputs_();

	void fillSchemeIO_();

	int getOutputOffset_(int i);
	void setInputOffset_(int i, int offset);

protected:
	wSimulationSystemScheme* s_;

	// Это - слепок схемы в более удобном для работы формате.
	// FIXME: Его создание занимает какое-то время, может лучше обойтись без него?
	struct block_output_
	{
		block_output_() : offset(-1) {}

		// connection
		ed::vector<std::pair<block_*, int> > cons;

		int offset;
	};

	struct block_input_
	{
		block_input_() : con_block(0), con_lead_idx(-1) {}

		// connection
		block_* con_block;
		int		con_lead_idx;

		int offset;
	};

	struct block_
	{
		block_() : desc(0), subbuilder(0) {};

		ed::vector<block_output_> outputs;
		ed::vector<block_input_> inputs;
		wSimulationBlockDescriptor* desc;
		wSimulationSystemLayoutBuilder* subbuilder;	// if typeof(desc) == wSimulationBlockSubsystem
	};

	ed::vector<block_> blocks_;
	block_ ioBlock_;
};


#endif 