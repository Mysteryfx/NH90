#ifndef __CASSETTE_OPEN_SUPRESSOR_H__
#define __CASSETTE_OPEN_SUPRESSOR_H__

#include "simulation_block_disc.h"

class wOpenSuppressorDescriptor;

class wOpenSuppressor : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wOpenSuppressor, wSimulationBlockDisc, wOpenSuppressorDescriptor);

public:
	wOpenSuppressor(const wOpenSuppressorDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time) { return -1; };

protected:
	void onOpen_ (bool);

protected:
	OUTPUT_PORT(bool,openOut_);
	INPUT_PORT(bool, openIn_);
	INPUT_PORT_CONST(bool, suppress_);
};

class wOpenSuppressorDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wOpenSuppressor);
public:
	virtual wOpenSuppressor* create(Lua::Loader* runtime_cfg, wSimulationSystem* data);
};

#endif
