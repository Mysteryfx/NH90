#ifndef __TrackTester__
#define __TrackTester__

#ifdef TRACK_TEST

#include "WorldGeneral.h"

namespace TrackTest
{

template<typename T>
struct FormatStr
{
	inline const char * get(int i) { return "%d"; }
	inline const char * get(unsigned int u) { return "%u"; }
#ifdef _WIN64
	inline const char * get(size_t u) { return "%u"; }
#endif
	inline const char * get(float f) { return "%f"; }
	inline const char * get(double d) { return "%f"; }
	inline const char * get(...) { return ""; }
};
template<typename T>
size_t printValue(char * buff, size_t size, const T & t)
{
	return ED_snprintf(buff, size, FormatStr<T>().get(t), t);
}

class Tester
{
public:
	Tester();
	~Tester();
	void WORLDGENERAL_API initialize(const char * trackFileName, ptrdiff_t depthIn, bool autoIn);
	void WORLDGENERAL_API setPause(bool pauseIn);
	void WORLDGENERAL_API release();
	bool WORLDGENERAL_API isInitialized() const;
	bool WORLDGENERAL_API isReady() const;
	template<typename T>
	inline bool test(const T & t, const char * name, T & value)
	{
		return test_(&t, sizeof(T), &value);
	}
	template<typename T>
	inline bool test(const T & t)
	{
		return test_(&t, sizeof(T));
	}
	void onDivergenceFound(const char * msg);
	static WORLDGENERAL_API Tester & instance();
private:
	bool WORLDGENERAL_API test_(const void * ptr, size_t size);
	size_t WORLDGENERAL_API test_(const void * ptr, size_t size, void * other);
private:
	class Impl;
	Impl * impl_;
};

class Printer
{
public:
	Printer()
	{
		clear();
	}
	void clear()
	{
		index_ = 0;
		buff[0] = '\0';
	}
	void print(const char * fmt, va_list args)
	{
		index_ += _vsnprintf(buff + index_, buffSizeMax - index_, fmt, args);
	}
	template<typename T>
	void print(const T & t, const char * name)
	{
		print_(t, name);
		index_ += ED_snprintf(buff + index_, buffSizeMax - index_, "\n");
	}
	template<typename T>
	void print(const T & t, const char * name, const T & other)
	{
		index_ += ED_snprintf(buff + index_, buffSizeMax - index_, "Error: ");
		print_(t, name);
		index_ += ED_snprintf(buff + index_, buffSizeMax - index_, " / ");
		index_ += printValue(buff + index_, buffSizeMax - index_, other);
		index_ += ED_snprintf(buff + index_, buffSizeMax - index_, "\n");
	}
	inline bool empty()
	{
		return index_ == 0;
	}
	inline const char * c_str() const
	{
		return buff;
	}
private:
	template<typename T>
	void print_(const T & t, const char * name)
	{
		index_ += ED_snprintf(buff + index_, buffSizeMax - index_, "%s = ", name);
		index_ += printValue(buff + index_, buffSizeMax - index_, t);
	}
	enum
	{
		buffSizeMax = 1024 * 4
	};
	size_t	index_;
	char	buff[buffSizeMax];
};

class Section
{
public:
	WORLDGENERAL_API Section(const char * file, int line, const char * func, bool checkPlace);
	WORLDGENERAL_API ~Section();
	void WORLDGENERAL_API line(int line, bool checkPlace);
	void WORLDGENERAL_API print(const char * fmt, ...);
	template<typename T>
	inline void testValue(int line, const T & t, const char * name)
	{
		if(TrackTest::Tester::instance().isReady())
		{
			T other;
			if(!testPosition(line))
			{
				errorInc_();
				print("Error: it is not %s\n", name);
			}
			else
			{
				if(!TrackTest::Tester::instance().test(t, name, other))
				{
					errorInc_();
					printer.print(t, name, other);
				}
				else
					printer.print(t, name);
			}
		}
	}
	template<typename T>
	inline bool testValue(const T & t)
	{
		if(TrackTest::Tester::instance().isReady())
			return TrackTest::Tester::instance().test(t);
		else
			return true;
	}
	template<typename T>
	inline const T & testExpr(int line, const T & t, const char * name)
	{
		testValue(line, t, name);
		return t;
	}
private:
	inline void errorInc_() { errorCounter++; }
	bool WORLDGENERAL_API testPosition(int line);
	static unsigned level_;
	size_t fileNameHash, funcNameHash;
	Printer printer;
	unsigned int errorCounter;
};

}

#define TRACK_TEST_SECTION TrackTest::Section __trackTestSection__(__FILE__, __LINE__, __FUNCTION__, true);
#define TRACK_TEST_SECTION_HELPER TrackTest::Section __trackTestSection__(__FILE__, __LINE__, __FUNCTION__, false);
#define TRACK_TEST_LINE __trackTestSection__.line(__LINE__, true);
#define TRACK_TEST_MARK_HELPER __trackTestSection__.line(__LINE__, false);
#define TRACK_TEST_HELPER(v) __trackTestSection__.print("%s", v);
#define TRACK_TEST_VALUE(v) __trackTestSection__.testValue(__LINE__, v, #v);
#define TRACK_TEST_EXPR(v) __trackTestSection__.testExpr(__LINE__, v, #v)

#else

#define TRACK_TEST_SECTION
#define TRACK_TEST_SECTION_HELPER
#define TRACK_TEST_LINE
#define TRACK_TEST_MARK_HELPER
#define TRACK_TEST_HELPER(v)
#define TRACK_TEST_VALUE(v)
#define TRACK_TEST_EXPR(v) v

#endif

#endif __TrackTester__