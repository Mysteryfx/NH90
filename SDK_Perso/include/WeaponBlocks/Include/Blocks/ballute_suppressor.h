#ifndef __BALLUTE_OPEN_SUPRESSOR_H__
#define __BALLUTE_OPEN_SUPRESSOR_H__

#include "simulation_block_disc.h"

class wBalluteSuppressorDescriptor;

class wBalluteSuppressor : public wSimulationBlockDisc
{
	DECLARE_BLOCK(wBalluteSuppressor, wSimulationBlockDisc, wBalluteSuppressorDescriptor);

public:
	wBalluteSuppressor(const wBalluteSuppressorDescriptor* desc, wSimulationSystem* system);

	virtual double simulate(double time);

protected:
	void onOpen_ (bool);

protected:
	OUTPUT_WIRES(1, (
		WIRE(bool, openBallute_)
		))

	INPUT_PORT(bool, openIn_);
	INPUT_PORT_CONST(bool, suppress_);
private:
	bool	m_openBallute;
};

class wBalluteSuppressorDescriptor : public wSimulationBlockDiscDescriptor
{
	DECLARE_BLOCK_DESCRIPTOR(wBalluteSuppressor);
public:
	virtual wBalluteSuppressor* create(Lua::Loader* runtime_cfg, wSimulationSystem* data);
	DECL_PARAM(double, dt);
};

#endif
