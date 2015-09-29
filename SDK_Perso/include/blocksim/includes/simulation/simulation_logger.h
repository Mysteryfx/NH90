#ifndef __SIMULATION_LOGGER_H__
#define __SIMULATION_LOGGER_H__

#include "../blocksim.h"

#include <ostream>
#include <ed/string.h>

// TODO: исправить логгер, чтобы в случае, когда ничего писать не надо было, аргументы бы не вычислялись
// http://www.rsdn.ru/Forum/?mid=2294753

enum weSimulationLogLevel
{
	SIMLOGLEV_NONE,
	SIMLOGLEV_SEVERE,
	SIMLOGLEV_MINOR,
	SIMLOGLEV_DEBUG
};

#define SIMLOG_OUT(logger, level) if ((level) > (logger).getLevel()); else (logger)

#define SIMLOG wSimulationLogger::instance()
#define SIMLOG_SEVERE SIMLOG_OUT(SIMLOG, SIMLOGLEV_SEVERE)
#define SIMLOG_MINOR  SIMLOG_OUT(SIMLOG, SIMLOGLEV_MINOR)
#define SIMLOG_DEBUG  SIMLOG_OUT(SIMLOG, SIMLOGLEV_DEBUG)

#define SIMLOG_INDENT wSimulationLogger::Indenter()

class BLOCKSIM_API wSimulationLogger
{
public:
	class Indenter {};

private:
	wSimulationLogger();
	~wSimulationLogger();

public:
	static wSimulationLogger& instance();

	void setStream(std::ostream* stream);
	void setLevel(weSimulationLogLevel level) { level_ = level; }
	weSimulationLogLevel getLevel() { return level_; }

	void setIndent(int cnt);
	int  getIndent();
	void addIndent(int cnt);

	template <class T> wSimulationLogger& operator << (T v) { (*s_) << v; return *this; }
	wSimulationLogger& operator << (std::ostream& (*v)(std::ostream&)) { (*s_) << v; return *this; }
	wSimulationLogger& operator << (const Indenter&) { (*s_) << indent_string_; return *this; }

protected:
	weSimulationLogLevel level_;
	std::ostream* s_;
	ed::string indent_string_;
};

#endif