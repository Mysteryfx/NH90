#pragma once

#include "_config.h"

namespace Common
{

typedef unsigned __int64 TimerValue;

class Profiler
{
public:
	Profiler();
	~Profiler();

	struct ED_CORE_EXTERN StatData
	{
		StatData();
		TimerValue ticks;
		int calls;
	};

	struct Data
	{
		Data();
		Data(const ed::string& name, int lastFramesCount);

		ed::string name;
		StatData statAll;
		StatData statCurFrame;

		ed::vector<StatData> lastFrames;
		std::size_t lastFilledFrameInd;
	};
	typedef ed::vector<Data> DataMap;

	struct CallData
	{
		CallData();
		CallData(TimerValue, Data*);
		TimerValue startTime;
		Data* data;
	};
	typedef ed::vector<CallData> ProfStack;

	ED_CORE_EXTERN int registerUnit(const ed::string& name);

	ED_CORE_EXTERN void startFrame();	
	ED_CORE_EXTERN void endFrame();

	ED_CORE_EXTERN void startProfiling(int id);
	ED_CORE_EXTERN void endProfiling();

	ED_CORE_EXTERN void activate(bool);
	inline bool activated() const {return _activated;};

	ED_CORE_EXTERN void setFrameCount(int);
	ED_CORE_EXTERN void reset();

	inline const  DataMap& currentData() const {return _dataMap;}
	ED_CORE_EXTERN double allTime() const;
	TimerValue timerFrequency() const {return _timerFreq;}
	const ed::vector<TimerValue> lastFramesTicks() const {return _lastFrames;}
	std::size_t lastFilledFrameIndex() const {return _lastFilledFrameInd;}
	int framesCount() const {return _lastFramesCount;}

private:
	DataMap _dataMap;
	ProfStack _profStack;
	std::size_t _stackInd;
	TimerValue _allTicks;
	ed::vector<TimerValue> _lastFrames;
	std::size_t _lastFilledFrameInd;
	TimerValue _startFrame;
	TimerValue _timerFreq;
	unsigned int _overhead;

	int _lastFramesCount;
	bool _activated;
	bool _isActiveNextFrame;
};

class ProfiledFunc
{
public:
	ED_CORE_EXTERN ProfiledFunc(int id);
	ED_CORE_EXTERN ~ProfiledFunc();
};
}

extern ED_CORE_EXTERN Common::Profiler globalProfiler;
